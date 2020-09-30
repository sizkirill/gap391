#include "MapGenerator.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Utils/Logger.h>
#include <Logic/Map/BspTree.h>
#include <Application/Resources/ResourceCache.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Utils/StringHash.h>

using namespace tinyxml2;

MapGenerator::MapGenerator(uint64_t seed)
    :m_rngdevice(seed)
{
}

bool MapGenerator::Init(std::string_view pathToSettings)
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

    XMLElement* pRoom = pRoot->FirstChildElement("Room");
    if (pRoom)
    {
        m_desiredRoomCount = pRoom->IntAttribute("count", kDesiredRoomCount);
    }

    XMLElement* pRoomWidthRange = pRoot->FirstChildElement("RoomWidthRange");
    if (pRoomWidthRange)
    {
        m_roomWidthRange = yang::IVectorFromXML(pRoomWidthRange);
    }

    XMLElement* pRoomHeightRange = pRoot->FirstChildElement("RoomHeightRange");
    if (pRoomHeightRange)
    {
        m_roomHeightRange = yang::IVectorFromXML(pRoomHeightRange);
    }

    XMLElement* pMapSettings = pRoot->FirstChildElement("Map");

    if (pMapSettings)
    {
        XMLElement* pMapSize = pMapSettings->FirstChildElement("Size");
        if (pMapSize)
        {
            m_mapSize = yang::IVectorFromXML(pMapSize);
        }

        XMLElement* pTileSize = pMapSettings->FirstChildElement("TileSize");
        if (pTileSize)
        {
            m_tileSize = yang::IVectorFromXML(pTileSize);
        }

        XMLElement* pStartPos = pMapSettings->FirstChildElement("StartPos");

        if (pStartPos)
        {
            m_startPos = yang::IVectorFromXML(pStartPos);
        }
    }

    XMLElement* pMapTileset = pRoot->FirstChildElement("Tileset");

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

            m_tileSpriteMap[StringHash32(pSpriteName)] = std::make_shared<yang::Sprite>(pTilesetTexture, yang::IRectFromXML(pSprite->FirstChildElement("SourceRect")), yang::TextureDrawParams{});
        }
    }

    return true;
}

DungeonMap MapGenerator::GenerateMap()
{
    DungeonMap generatedMap;
    generatedMap.Init(m_mapSize, m_tileSize, m_startPos, m_tileSpriteMap);

    std::shared_ptr<BspTreeNode> pRoot = std::make_shared<BspTreeNode>(yang::IRect{ 0,0,m_mapSize.x, m_mapSize.y }, &m_rngdevice, m_roomHeightRange, m_roomWidthRange);
    auto pRoomNodes = BspTreeNode::Split(pRoot.get(), m_desiredRoomCount);
    generatedMap.GenerateRoomGraph(pRoot.get());

    generatedMap.SetMapTree(pRoot);
    generatedMap.SetLeaves(std::move(pRoomNodes));

    return generatedMap;
}

bool MapGenerator::IsDone() const
{
    if (m_currentRoomCount >= m_desiredRoomCount)
        return true;
    return false;
}
