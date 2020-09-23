#include "MapGenerator.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Utils/Logger.h>

using namespace tinyxml2;

enum class Direction
{
    kRight,
    kDown,
    kLeft,
    kUp,
    MAX_DIRECTIONS
};

static yang::IVec2 Vec2FromDirection(Direction dir)
{
    switch (dir)
    {
    case Direction::kRight: return { 1,0 };
    case Direction::kLeft: return { -1,0 };
    case Direction::kDown: return { 0,1 };
    case Direction::kUp: return { 0,-1 };
    default: return { 0,0 };
    }
}

static yang::IVec2 RandomDirection(yang::XorshiftRNG& rngDevice)
{
    return Vec2FromDirection(static_cast<Direction>(rngDevice.Rand(static_cast<int>(Direction::MAX_DIRECTIONS))));
}

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

    XMLElement* pTurnSettings = pRoot->FirstChildElement("TurnSettings");

    if (pTurnSettings)
    {
        m_initialTurnChance = pTurnSettings->IntAttribute("chance", kDefaultTurnChance);
        m_turnChanceStep = pTurnSettings->IntAttribute("step", kDefaultTurnChanceStep);
    }

    XMLElement* pRoomSettings = pRoot->FirstChildElement("RoomSettings");

    if (pRoomSettings)
    {
        m_initialRoomChance = pRoomSettings->IntAttribute("chance", kDefaultRoomChance);
        m_roomChanceStep = pRoomSettings->IntAttribute("step", kDefaultRoomChanceStep);
    }

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

    return true;
}

DungeonMap MapGenerator::GenerateMap()
{
    DungeonMap generatedMap;
    generatedMap.Init(m_mapSize, m_tileSize, m_startPos);

    yang::IVec2 currentDirection = RandomDirection(m_rngdevice);

    int roomChance = m_initialRoomChance;
    int turnChance = m_initialTurnChance;

    yang::Vector2<int> diggerPos = m_startPos;

    assert(diggerPos.x >= 0 && diggerPos.x < m_mapSize.x && diggerPos.y >= 0 && diggerPos.y < m_mapSize.y);
    generatedMap.SetTile(diggerPos, DungeonMap::TileType::kTunnel);

    while (!IsDone())
    {
        diggerPos += currentDirection;

        while (diggerPos.x >= m_mapSize.x - 1 || diggerPos.y >= m_mapSize.y - 1 || diggerPos.x < 1 || diggerPos.y < 1)
        {
            diggerPos = diggerPos - currentDirection;
            currentDirection = RandomDirection(m_rngdevice);
            diggerPos += currentDirection;
        }

        generatedMap.SetTile(diggerPos, DungeonMap::TileType::kTunnel);

        int choice = m_rngdevice.Rand(100);
        if (choice < turnChance)
        {
            currentDirection = RandomDirection(m_rngdevice);
            turnChance = 0;
        }
        else
        {
            turnChance += 5;
        }

        choice = m_rngdevice.Rand(100);

        if (choice < roomChance)
        {
            int width = m_rngdevice.Rand(m_roomWidthRange.x, m_roomWidthRange.y);
            int height = m_rngdevice.Rand(m_roomHeightRange.x, m_roomHeightRange.y);
        
            generatedMap.PlaceRoom(diggerPos, { width, height });
            ++m_currentRoomCount;

            roomChance = 0;
        }
        else
        {
            roomChance += 5;
        }
    }

    return generatedMap;
}

bool MapGenerator::IsDone() const
{
    if (m_currentRoomCount >= m_desiredRoomCount)
        return true;
    return false;
}
