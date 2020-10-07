#include "WorldMap.h"
#include <Utils/Random.h>
#include <Utils/Logger.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/ApplicationConstants.h>
#include <Utils/Rectangle.h>
#include <Logic/Event/Input/KeyboardInputEvent.h>
#include <Logic/Event/EventDispatcher.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Utils/StringHash.h>
#include <Application/Resources/ResourceCache.h>
#include <Application/Graphics/Textures/Sprite.h>
#include <Logic/Event/Input/MouseWheelEvent.h>
#include <thread>
#include <cassert>

WorldMap::~WorldMap()
{
    yang::EventDispatcher::Get()->RemoveEventListener(yang::KeyboardInputEvent::kEventId, m_keyboardListenerIndex);
}

bool WorldMap::Init(yang::IVec2 mapSize, yang::IVec2 tileSize, yang::FVec2 noiseSize, yang::IGraphics* pGraphics)
{
    m_mapSize = mapSize;
    m_tileSize = tileSize;
    m_elevationMap.Init(mapSize, tileSize, noiseSize);
    m_moistureMap.Init(mapSize, tileSize, noiseSize);
    m_center = mapSize / 2;
    m_pGraphics = pGraphics;
    m_biomes.resize(mapSize.x * mapSize.y);
    m_numOctaves = 4;
    m_persistance = 0.5f;
    m_keyboardListenerIndex = yang::EventDispatcher::Get()->AddEventListener(yang::KeyboardInputEvent::kEventId, [this](yang::IEvent* pEvent) {HandleInputEvent(pEvent); });
    //m_mouseListenerIndex = yang::EventDispatcher::Get()->AddEventListener(yang::MouseWheelEvent::kEventId, [this](yang::IEvent* pEvent) {HandleWheelEvent(pEvent); });
    return true;
}

bool WorldMap::Init(std::string_view pathToSettings)
{
    using namespace tinyxml2;

    XMLDocument doc;
    XMLError error = doc.LoadFile(pathToSettings.data());

    if (error != tinyxml2::XML_SUCCESS)
    {
        LOG(Error, "Failed to load map settings: %s -- %s", pathToSettings.data(), XMLDocument::ErrorIDToName(error));
        return false;
    }

    XMLElement* pRoot = doc.RootElement();

    XMLElement* pMapSize = pRoot->FirstChildElement("MapSize");
    if (pMapSize)
    {
        m_mapSize = yang::IVectorFromXML(pMapSize);
    }

    XMLElement* pTileSize = pRoot->FirstChildElement("TileSize");
    if (pTileSize)
    {
        m_tileSize = yang::FVectorFromXML(pTileSize);
    }

    XMLElement* pNoiseSize = pRoot->FirstChildElement("NoiseSize");
    if (pNoiseSize)
    {
        m_noiseSize = yang::FVectorFromXML(pNoiseSize);
    }

    m_elevationMap.Init(m_mapSize, m_tileSize, m_noiseSize);
    m_moistureMap.Init(m_mapSize, m_tileSize, m_noiseSize);
    m_center = m_mapSize / 2;
    m_biomes.resize(m_mapSize.x * m_mapSize.y);

    m_numOctaves = pRoot->IntAttribute("octaves", 4);
    m_persistance = pRoot->FloatAttribute("persistance", 0.5f);

    //float scale = pRoot->FloatAttribute("initialScale", 1.f);
    //m_tileScaleFactors = { scale,scale };

    m_keyboardListenerIndex = yang::EventDispatcher::Get()->AddEventListener(yang::KeyboardInputEvent::kEventId, [this](yang::IEvent* pEvent) {HandleInputEvent(pEvent); });
    //m_mouseListenerIndex = yang::EventDispatcher::Get()->AddEventListener(yang::MouseWheelEvent::kEventId, [this](yang::IEvent* pEvent) {HandleWheelEvent(pEvent); });

    XMLElement* pMapTileset = pRoot->FirstChildElement("Tileset");

    using namespace yang::literals;
    static constexpr uint32_t tileNames[static_cast<size_t>(Biome::kMaxBiomes) + 1] = 
    { 
    "ocean"_hash32,
    "beach"_hash32,
    "tundra"_hash32,
    "temperateSeasonalForest"_hash32,
    "grassland"_hash32,
    "borealForest"_hash32,
    "shrubland"_hash32,
    "temperateRainforest"_hash32,
    "tropicalRainforest"_hash32,
    "savanna"_hash32,
    "subtropicalDesert"_hash32,
    "snow"_hash32,
    "default"_hash32
    };

    auto spriteLambda = [](std::string_view spriteName, uint32_t defaultVal)
    {
        uint32_t hashVal = StringHash32(spriteName.data());
        auto ptr = std::find(tileNames, tileNames + static_cast<size_t>(Biome::kMaxBiomes), hashVal);
        return ptr - tileNames;
    };

    if (pMapTileset)
    {
        const char* pTextureSrc = pMapTileset->Attribute("src");

        if (!pTextureSrc)
        {
            LOG(Error, "Texture haven't been found");
            return false;
        }

        auto pTilesetTexture = yang::ResourceCache::Get()->Load<yang::ITexture>(pTextureSrc);

        for (XMLElement* pSprite = pMapTileset->FirstChildElement("Sprite"); pSprite != nullptr; pSprite = pSprite->NextSiblingElement("Sprite"))
        {
            const char* pSpriteName = pSprite->Attribute("name");
            if (!pSpriteName)
            {
                LOG(Warning, "Sprite doesn't have a name");
                continue;
            }

            m_biomeToSpriteArray[spriteLambda(pSpriteName, "default"_hash32)] = std::make_shared<yang::Sprite>(pTilesetTexture, yang::IRectFromXML(pSprite->FirstChildElement("SourceRect")), yang::TextureDrawParams{});
        }
    }
    return true;
}

void WorldMap::Generate(uint64_t seed)
{
    yang::XorshiftRNG rng(seed);
    auto elevationSeed = rng();
    auto moistureSeed = rng();

    std::vector<float> elevationValues;
    std::vector<float> moistureValues;

    auto start = std::chrono::steady_clock::now();

    std::thread elevationsJob([this, elevationSeed, &elevationValues]() {elevationValues = m_elevationMap.Generate(static_cast<uint32_t>(elevationSeed), m_numOctaves, m_persistance); });
    std::thread moisturesJob([this, moistureSeed, &moistureValues]() {moistureValues = m_elevationMap.Generate(static_cast<uint32_t>(moistureSeed), m_numOctaves, m_persistance); });

    elevationsJob.join();
    moisturesJob.join();

    AssignBiomes(elevationValues, moistureValues);

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<float> duration = end - start;
    LOG(Info, "Map generation took %f seconds", duration.count());
}

bool WorldMap::Render(yang::IGraphics* pGraphics) const
{
    for (int y = 0; y < m_mapSize.y; ++y)
    {
        for (int x = 0; x < m_mapSize.x; ++x)
        {
            yang::IRect toDraw;
            toDraw.x = x * m_tileSize.x;
            toDraw.y = y * m_tileSize.y;
            toDraw.width = m_tileSize.x;
            toDraw.height = m_tileSize.y;
            pGraphics->DrawSprite(GetSpriteFromBiome(m_biomes[GetIndexFromGridPoint(x, y)]), toDraw);
        }
    }

    return true;
}

void WorldMap::AssignBiomes(const std::vector<float>& elevations, const std::vector<float>& moistures)
{
    static constexpr size_t kNumJobs = 8;
    std::thread jobs[kNumJobs];
    size_t jobSize = elevations.size() / kNumJobs;
    for (size_t i = 0; i < kNumJobs; ++i)
    {
        size_t startIndex = i * jobSize;
        size_t endIndex = (i == kNumJobs - 1 ? elevations.size() : (i + 1) * jobSize);
        jobs[i] = std::thread([startIndex, endIndex, &elevations, &moistures, this]()
            {
                for (size_t i = startIndex; i < endIndex; ++i)
                {
                    float e = (1.f + elevations[i] - DistanceToCenter(static_cast<int>(i))) / 2.f;
                    m_biomes[i] = BiomeFromNoise(e, moistures[i]);
                }
            });
    }

    for (auto& t : jobs)
    {
        t.join();
    }
}

void WorldMap::AssignBiom(size_t index, float elevation, float moisture)
{

}

Biome WorldMap::BiomeFromNoise(float elevation, float moisture)
{
    if (elevation < 0.45f) return Biome::kOcean;
    if (elevation < 0.48f) return Biome::kBeach;

    if (elevation > 0.78f)
    {
        if (moisture < 0.6f) return Biome::kTundra;
        return Biome::kSnow;
    }
    if (elevation > 0.67f) 
    {
        if (moisture < 0.5f) return Biome::kShrubland;
        if (moisture < 0.7f) return Biome::kGrassland;
        return Biome::kTemperateSeasonalForest;
    }
    if (elevation > 0.55f) {
        if (moisture < 0.3f) return Biome::kSubtropicalDesert;
        if (moisture < 0.45f) return Biome::kShrubland;
        if (moisture < 0.65f) return Biome::kGrassland;
        return Biome::kTemperateRainforest;
    }

    if (moisture < 0.3f) return Biome::kSubtropicalDesert;
    if (moisture < 0.5f) return Biome::kSavanna;
    if (moisture < 0.67f) return Biome::kTemperateRainforest;
    return Biome::kTropicalRainForest;
} 

int WorldMap::GetIndexFromGridPoint(int x, int y) const
{
    return y * m_mapSize.x + x;
}

int WorldMap::GetIndexFromGridPoint(yang::IVec2 gridPoint) const
{
    return GetIndexFromGridPoint(gridPoint.x, gridPoint.y);
}

yang::IVec2 WorldMap::GetGridPointFromIndex(int index) const
{
    return { index % m_mapSize.x, index / m_mapSize.x };
}

float WorldMap::DistanceToCenter(int index) const
{
    yang::FVec2 current = GetGridPointFromIndex(index);

    yang::FVec2 delta = { current.x / m_mapSize.x - 0.5f, current.y / m_mapSize.y - 0.5f };

    float v = 2 * std::max(std::fabs(delta.x), std::fabs(delta.y));

    return v;

    // Tweak for better results ?
    //return std::powf(v, 0.85f);
}

float WorldMap::GetRadiusSqrd() const
{
    return yang::FVec2(m_mapSize / 2).SqrdLength();
}

bool WorldMap::CheckBounds(int x, int y) const
{
    return !(x < 0 || x >= m_mapSize.x || y < 0 || y >= m_mapSize.y);
}

std::shared_ptr<yang::Sprite> WorldMap::GetSpriteFromBiome(Biome biome) const
{
    return m_biomeToSpriteArray[static_cast<size_t>(biome)];
}

void WorldMap::HandleInputEvent(yang::IEvent* pEvent)
{
    if (pEvent->GetEventId() == yang::KeyboardInputEvent::kEventId)
    {
        auto pKeyboardEvent = static_cast<yang::KeyboardInputEvent*>(pEvent);
        if (pKeyboardEvent->GetEventType() == yang::KeyboardInputEvent::EventType::kKeyReleased)
        {
            if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kSPACE)
            {
                Generate(m_rng());
            }

            else if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kLSHIFT)
            {
                ++m_numOctaves;
                LOG(Info, "NumOctaves: %d", m_numOctaves);
                Generate(m_rng.GetState());
            }

            else if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kLCTRL)
            {
                --m_numOctaves;
                if (m_numOctaves < 1) m_numOctaves = 1;
                LOG(Info, "NumOctaves: %d", m_numOctaves);
                Generate(m_rng.GetState());
            }

            else if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::k0)
            {
                m_persistance = std::clamp(m_persistance + 0.1f, 0.f, 4.f);
                LOG(Info, "Persistance: %f", m_persistance);
                Generate(m_rng.GetState());
            }

            else if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::k9)
            {
                m_persistance = std::clamp(m_persistance - 0.1f, 0.f, 4.f);
                LOG(Info, "Persistance: %f", m_persistance);
                Generate(m_rng.GetState());
            }
        }
    }
}

//void WorldMap::HandleWheelEvent(yang::IEvent* pEvent)
//{
//    using namespace yang;
//
//    assert(MouseWheelEvent::kEventId == pEvent->GetEventId());
//
//    MouseWheelEvent* pResult = static_cast<MouseWheelEvent*>(pEvent);
//
//    float amount = pResult->GetScrollAmount().y;
//    m_tileScaleFactors *= (1 + amount / 10.f);
//}
