#include "TiledMap.h"
#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Application/Resources/ResourceCache.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Utils/StringHash.h>

#include <charconv>
#include <cassert>

using yang::TiledMap;

static constexpr uint32_t kBoolProperty = StringHash32("bool");
static constexpr uint32_t kStringProperty = 0;
static constexpr uint32_t kIntProperty = StringHash32("int");
static constexpr uint32_t kFloatProperty = StringHash32("float");

template <>
void yang::TiledMap::TilesetData::AddProperty<bool>(const char* name, bool value)
{
    auto& propertyMap = *(--m_tileProperties.end());
    propertyMap[StringHash32(name)] = value;
}

template <>
void yang::TiledMap::TilesetData::AddProperty<const char*>(const char* name, const char* value)
{
    auto& propertyMap = *(--m_tileProperties.end());
    propertyMap[StringHash32(name)] = std::string(value);
}

template <>
void yang::TiledMap::TilesetData::AddProperty<int>(const char* name, int value)
{
    auto& propertyMap = *(--m_tileProperties.end());
    propertyMap[StringHash32(name)] = value;
}

template <>
void yang::TiledMap::TilesetData::AddProperty<float>(const char* name, float value)
{
    auto& propertyMap = *(--m_tileProperties.end());
    propertyMap[StringHash32(name)] = value;
}

template<>
std::optional<bool> yang::TiledMap::TilesetData::GetProperty(int tileId, const char* propertyName) const
{
    assert(tileId < m_tileCount);
    assert(propertyName);

    if (static_cast<size_t>(tileId) >= m_tileProperties.size())
    {
        return {};
    }

    uint32_t hashName = StringHash32(propertyName);

    if (m_tileProperties[tileId].count(hashName))
    {
        if (const bool* pVal = std::get_if<bool>(&m_tileProperties[tileId].at(hashName)))
        {
            return *pVal;
        }

        LOG(Error, "Tile property with name %s is not a boolean", propertyName);
        return {};
    }

    //LOG(Error, "Tile doesn't have property with name %s", propertyName);

    return {};
}

template<>
std::optional<std::string> yang::TiledMap::TilesetData::GetProperty(int tileId, const char* propertyName) const
{
    assert(tileId < m_tileCount);
    assert(propertyName);

    if (static_cast<size_t>(tileId) >= m_tileProperties.size())
    {
        return {};
    }

    uint32_t hashName = StringHash32(propertyName);

    if (m_tileProperties[tileId].count(hashName))
    {
        if (const std::string* pVal = std::get_if<std::string>(&m_tileProperties[tileId].at(hashName)))
        {
            return *pVal;
        }

        LOG(Error, "Tile property with name %s is not a string", propertyName);
        return {};
    }

    //LOG(Error, "Tile doesn't have property with name %s", propertyName);

    return {};
}

template<>
std::optional<int> yang::TiledMap::TilesetData::GetProperty(int tileId, const char* propertyName) const
{
    assert(tileId < m_tileCount);
    assert(propertyName);

    if (static_cast<size_t>(tileId) >= m_tileProperties.size())
    {
        return {};
    }

    uint32_t hashName = StringHash32(propertyName);

    if (m_tileProperties[tileId].count(hashName))
    {
        if (const int* pVal = std::get_if<int>(&m_tileProperties[tileId].at(hashName)))
        {
            return *pVal;
        }

        LOG(Error, "Tile property with name %s is not a integer", propertyName);
        return {};
    }

    //LOG(Error, "Tile doesn't have property with name %s", propertyName);

    return {};
}

template<>
std::optional<float> yang::TiledMap::TilesetData::GetProperty(int tileId, const char* propertyName) const
{
    assert(tileId < m_tileCount);
    assert(propertyName);

    if (static_cast<size_t>(tileId) >= m_tileProperties.size())
    {
        return {};
    }

    uint32_t hashName = StringHash32(propertyName);

    if (m_tileProperties[tileId].count(hashName))
    {
        if (const float* pVal = std::get_if<float>(&m_tileProperties[tileId].at(hashName)))
        {
            return *pVal;
        }

        LOG(Error, "Tile property with name %s is not a float", propertyName);
        return {};
    }

    //LOG(Error, "Tile doesn't have property with name %s", propertyName);

    return {};
}

bool yang::TiledMap::LoadMap(const char* filepath)
{
    using namespace tinyxml2;
    // Open 2 files:
    std::string mapFile(filepath);
    XMLDocument mapDocument;
    XMLError error = mapDocument.LoadFile(mapFile.c_str());
    if (error != XML_SUCCESS)
    {
        LOG(Error, "Failed to load map file. Path: %s. Error: %s", filepath, XMLDocument::ErrorIDToName(error));
        return false;
    }

    XMLElement* pMapRoot = mapDocument.RootElement();

    // TODO: Error checks for each (or leave it to tiled?)
    m_mapData.m_mapWidth = pMapRoot->IntAttribute("width");
    m_mapData.m_mapHeight = pMapRoot->IntAttribute("height");
    m_mapData.m_tileWidth = pMapRoot->IntAttribute("tilewidth");
    m_mapData.m_tileHeight = pMapRoot->IntAttribute("tileheight");

	std::string orientation = pMapRoot->Attribute("orientation");
	if (orientation == "isometric")
	{
		m_mapData.m_mapType = MapType::kIsometric;
	}
	else if (orientation == "orthogonal")
	{
		m_mapData.m_mapType = MapType::kOrthogonal;
	}
	else
	{
		m_mapData.m_mapType = MapType::kMaxTypes;
	}

    // Can contain N tilesets, we *should* loop through siblings
    for (XMLElement* pTileset = pMapRoot->FirstChildElement("tileset"); pTileset != nullptr; pTileset = pTileset->NextSiblingElement("tileset"))
    {
        int firstGid = pTileset->IntAttribute("firstgid");

        std::string tilesetXmlPath = pTileset->Attribute("source");

        // Open our second file
        std::string tilesetFile = mapFile.substr(0, mapFile.rfind('/') + 1);
        tilesetFile += tilesetXmlPath;

        XMLDocument tilesetDocument;
        error = tilesetDocument.LoadFile(tilesetFile.c_str());

        if (error != XML_SUCCESS)
        {
            LOG(Error, "Failed to load tileset file. Path: %s. Error: %s", tilesetFile.c_str(), XMLDocument::ErrorIDToName(error));
            return false;
        }

        XMLElement* pTilesetRoot = tilesetDocument.RootElement();

        TilesetData tilesetData(firstGid, std::move(tilesetFile), pTilesetRoot);

        m_tilesetData.push_back(std::move(tilesetData));
    }

    for (XMLElement* pLayer = pMapRoot->FirstChildElement("layer"); pLayer != nullptr; pLayer = pLayer->NextSiblingElement("layer"))
    {
        LayerData layerData;

        layerData.m_id = pLayer->IntAttribute("id");
        
        // Do we need width and height? I don't think they can be different from map width/height
        layerData.m_width = pLayer->IntAttribute("width");
        layerData.m_height = pLayer->IntAttribute("height");

        layerData.m_opacity = pLayer->FloatAttribute("opacity", 1.f);
        layerData.m_isVisible = pLayer->BoolAttribute("visible", true);

        const char* pName = pLayer->Attribute("name");
        if (pName)
        {
            layerData.m_hashName = StringHash32(pName);
        }

        // TODO: parse properties

        int mapSize = layerData.m_width * layerData.m_height;

        layerData.m_tileData.reserve(mapSize);

        XMLElement* pData = pLayer->FirstChildElement("data");
        std::string dataString = pData->GetText();

        // C++17 claims that std::from_chars and to_chars are lightning fast.
        size_t currentComma = 0;
        size_t nextComma = dataString.find(",");
        const char* dataCString = dataString.c_str();

        for (int i = 0; i < mapSize; ++i)
        {
            // If next comma wasn't found, parse until the end of string - 1 (because of '\n' at the end)
            if (nextComma == std::string::npos)
            {
                nextComma = dataString.size() - 1;
            }

            // If there is a '\n' character between current and next, we need to increment currentComma ('\n' character is guaranteed to be next after currentComma)
            // Note that this if should be after the previous if, otherwise it will catch the '\n' at the end of the whole string.
            if (dataString.find("\n", currentComma) < nextComma)
            {
                ++currentComma;
            }

            int id;
            std::from_chars(dataCString + currentComma, dataCString + nextComma, id);
            currentComma = nextComma + 1;
            nextComma = dataString.find(",", currentComma);

            layerData.m_tileData.push_back({id});
        }
        m_mapData.m_layerData.push_back(std::move(layerData));
    }

    return true;
}

bool yang::TiledMap::Render(IGraphics* pGraphics)
{
    bool success = true;
    for (auto& layer : m_mapData.m_layerData)
    {
        if (!layer.m_isVisible)
        {
            continue;
        }

        for (auto tileIt = layer.m_tileData.cbegin(); tileIt != layer.m_tileData.cend(); ++tileIt)
        {
            // if tile id is 0, it is an empty tile. We could've ignored them, but this way it is easier to calculate
            //      world point, and that way we don't store X and Y position of the tile.
            if (tileIt->m_id == 0)
            {
                continue;
            }

            // Finding the tileset, where the tile lives. our m_tilesetData is sorted by firstGid (prove me wrong!), 
            //      so upper_bound will give us iterator to the next tileset (or end if the needed tileset is the last one)
            auto tilesetIt = std::upper_bound(m_tilesetData.begin(), m_tilesetData.end(), tileIt->m_id, [](int firstgid, const TilesetData& tilesetData)
                {
                    return firstgid < tilesetData.GetFirstGid();
                });

            // Getting the needed tileset
            --tilesetIt;

            tilesetIt->GetTilesetTexture()->SetAlpha(layer.m_opacity);

            size_t tileIndex = static_cast<size_t>(tileIt->m_id) - tilesetIt->GetFirstGid();

            int xCoord = static_cast<int>(tileIt - layer.m_tileData.cbegin()) % m_mapData.m_mapWidth;
            int yCoord = static_cast<int>(tileIt - layer.m_tileData.cbegin()) / m_mapData.m_mapWidth;

            success = tilesetIt->RenderTile(pGraphics, tileIndex, IVec2(xCoord,yCoord));

            if (!success)
                break;
        }

        if (!success)
            break;
    }

    for (const auto& tilesetData : m_tilesetData)
    {
        tilesetData.GetTilesetTexture()->SetAlpha(1.f);
    }

    return success;
}

yang::FVec2 yang::TiledMap::GetWorldPointFromTileIndex(size_t index) const
{
    auto coords = GetCoordsFromIndex(index);
    return FVec2((0.5f + coords.x) * m_mapData.m_tileWidth, (0.5f + coords.y) * m_mapData.m_tileHeight);
    //return FVec2(IVec2::Scale(GetCoordsFromIndex(index), IVec2(m_mapData.m_tileWidth, m_mapData.m_tileHeight)));
}

size_t yang::TiledMap::GetTileIndexFromWorldPoint(FVec2 position) const
{
    int x = static_cast<int>(position.x / m_mapData.m_tileWidth);
    int y = static_cast<int>(position.y / m_mapData.m_tileHeight);
    return GetIndexFromCoords(IVec2(x,y));
}

yang::Vector2<size_t> yang::TiledMap::GetCoordsFromIndex(size_t index) const
{
    assert(m_mapData.m_mapWidth != 0 && m_mapData.m_mapHeight != 0);
    return Vector2<size_t>(index % m_mapData.m_mapWidth, index / m_mapData.m_mapWidth);
}

size_t yang::TiledMap::GetIndexFromCoords(Vector2<size_t> coords) const
{
    return coords.y * m_mapData.m_mapWidth + coords.x;
}

size_t yang::TiledMap::GetTileIdFromIndex(size_t layerIndex, size_t index) const
{
    return static_cast<size_t>(m_mapData.m_layerData[layerIndex].m_tileData[index].m_id) - 1;
}

void yang::TiledMap::SetTileIdAtIndex(size_t layerIndex, size_t tileIndex, size_t tileId)
{
    m_mapData.m_layerData[layerIndex].m_tileData[tileIndex].m_id = static_cast<int>(tileId);
}

size_t yang::TiledMap::GetLayerIndex(const char* pName)
{
    assert(pName);
    uint32_t hash = StringHash32(pName);
    auto it = std::find_if(m_mapData.m_layerData.begin(), m_mapData.m_layerData.end(), [hash](const LayerData& layerData) {return layerData.m_hashName == hash; });
    if (it != m_mapData.m_layerData.end())
    {
        return it - m_mapData.m_layerData.begin();
    }

    return kInvalidValue<size_t>;
}

yang::TiledMap::TilesetData::TilesetData(int firstGid, std::string&& source, tinyxml2::XMLElement* pData)
    :m_firstGid(firstGid)
    ,m_source(std::move(source))
{
    ReadTilesetData(pData);
}

void yang::TiledMap::TilesetData::ReadTilesetData(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    XMLElement* pTilesetImage = pData->FirstChildElement("image");

    m_columns = pData->IntAttribute("columns");
    m_tileWidth = pData->IntAttribute("tilewidth");
    m_tileHeight = pData->IntAttribute("tileheight");
    m_tileCount = pData->IntAttribute("tilecount");

    m_source = m_source.substr(0, m_source.rfind('/') + 1) + pTilesetImage->Attribute("source");
    m_imageWidth = pTilesetImage->IntAttribute("width");
    m_imageHeight = pTilesetImage->IntAttribute("height");
    m_pTilesetTexture = ResourceCache::Get()->Load<ITexture>(m_source.c_str());

    for (XMLElement* pTile = pData->FirstChildElement("tile"); pTile != nullptr; pTile = pTile->NextSiblingElement("tile"))
    {
        XMLElement* pProperties = pTile->FirstChildElement("properties");
        if (pProperties)
        {
            // emplacing an empty property map
            m_tileProperties.emplace_back();

            for (XMLElement* pProperty = pProperties->FirstChildElement("property"); pProperty != nullptr; pProperty = pProperty->NextSiblingElement("property"))
            {
                const char* pType = pProperty->Attribute("type");
                uint32_t propertyType = 0;
                if (pType)
                {
                    propertyType = StringHash32(pType);
                }
                
                switch (propertyType)
                {
                case kBoolProperty:
                    AddProperty(pProperty->Attribute("name"), pProperty->BoolAttribute("value"));
                    break;
                case kStringProperty:
                    AddProperty(pProperty->Attribute("name"), pProperty->Attribute("value"));
                    break;
                case kIntProperty:
                    AddProperty(pProperty->Attribute("name"), pProperty->IntAttribute("value"));
                    break;
                case kFloatProperty:
                    AddProperty(pProperty->Attribute("name"), pProperty->FloatAttribute("value"));
                    break;
                default:
                    LOG(Error, "Reached default in parsing tile properties. Property has unknown type or not supported");
                    break;
                }
            }

        }
    }
}

bool yang::TiledMap::TilesetData::RenderTile(IGraphics* pGraphics, size_t tileIndex, IVec2 dstPos) const
{
    int srcX = (int)tileIndex % m_columns;
    int srcY = (int)tileIndex / m_columns;
    yang::IRect srcRect(srcX * m_tileWidth, srcY * m_tileHeight, m_tileWidth, m_tileHeight);
    IRect dstRect = { dstPos.x * m_tileWidth, dstPos.y * m_tileHeight, m_tileWidth, m_tileHeight };
    return pGraphics->DrawTexture(m_pTilesetTexture.get(), srcRect, dstRect);
}

yang::ITexture* yang::TiledMap::TilesetData::GetTilesetTexture() const
{
    return m_pTilesetTexture.get();
}
