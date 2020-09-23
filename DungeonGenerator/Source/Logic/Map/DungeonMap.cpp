#include "DungeonMap.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Utils/XMLHelpers.h>
#include <Utils/Rectangle.h>
#include <unordered_set>
#include <queue>
#include <cassert>
#include <algorithm>

DungeonMap::DungeonMap(yang::IVec2 mapSize)
    :m_mapSize(mapSize)
{
}

bool DungeonMap::Init(yang::IVec2 mapSize, yang::IVec2 tileSize, yang::IVec2 startPos)
{
    m_mapSize = mapSize;
    m_tileSize = tileSize;
    m_startPos = startPos;

    m_tileset[IndexFromTileType(TileType::kWall)] = yang::IColor(0x000000ff);
    m_tileset[IndexFromTileType(TileType::kTunnel)] = yang::IColor(127, 127, 127, 255);
    m_tileset[IndexFromTileType(TileType::kPlayer)] = yang::IColor(200, 50, 50, 255);
    m_tileset[IndexFromTileType(TileType::kDoor)] = yang::IColor(255, 124, 0, 255);

    Tile wall{ TileType::kWall, m_tileset[static_cast<size_t>(TileType::kWall)] };
    m_map = std::vector<Tile>(m_mapSize.x * m_mapSize.y, wall);

    return true;
}

void DungeonMap::SetTile(yang::IVec2 position, TileType tileType)
{
    m_map[GetIndexFromGridPoint(position)].m_tileType = tileType;
    m_map[GetIndexFromGridPoint(position)].m_color = m_tileset[static_cast<size_t>(tileType)];
}

void DungeonMap::SetTile(size_t index, TileType tileType)
{
    m_map[index].m_tileType = tileType;
    m_map[index].m_color = m_tileset[static_cast<size_t>(tileType)];
}

void DungeonMap::PlaceRoom(yang::IVec2 center, yang::IVec2 dimensions)
{
    int startX = std::clamp(center.x - dimensions.x / 2, 1, m_mapSize.x - 2);
    int startY = std::clamp(center.y - dimensions.y / 2, 1, m_mapSize.y - 2);
    int endX = std::clamp(center.x + dimensions.x / 2 + dimensions.x % 2, 1, m_mapSize.x - 2);
    int endY = std::clamp(center.y + dimensions.y / 2 + dimensions.y % 2, 1, m_mapSize.y - 2);

    Tile room{ TileType::kTunnel, m_tileset[static_cast<size_t>(TileType::kTunnel)] };
    for (int y = startY; y != endY; ++y)
    {
        std::fill(m_map.begin() + GetIndexFromGridPoint({ startX, y }), m_map.begin() + GetIndexFromGridPoint({ endX,y }), room);
    }
}

bool DungeonMap::Render(yang::IGraphics* pGraphics) const
{
    bool success = true;
    for (size_t i = 0; i < m_map.size(); ++i)
    {
        auto point = GetGridPointFromIndex(i);
        yang::IRect target;
        target.x = point.x * m_tileSize.x;
        target.y = point.y * m_tileSize.y;
        target.width = m_tileSize.x;
        target.height = m_tileSize.y;
        success = pGraphics->FillRect(target, m_map[i].m_color);
    }

    for (auto& room : m_roomGraph.m_rooms)
    {
        size_t roomColorIndex = room.m_colorIndex;

        for (auto& index : room.m_tileIndices)
        {
            auto point = GetGridPointFromIndex(index);
            yang::IRect target;
            target.x = point.x * m_tileSize.x + m_tileSize.x / 4;
            target.y = point.y * m_tileSize.y + m_tileSize.y / 4;
            target.width = m_tileSize.x - m_tileSize.x / 2;
            target.height = m_tileSize.y - m_tileSize.y / 2;
            success = pGraphics->FillRect(target, m_roomColors[roomColorIndex % m_roomColors.size()]);
        }
    }
    return success;
}

void DungeonMap::Update(float deltaSeconds)
{
}

size_t DungeonMap::GetIndexFromGridPoint(yang::IVec2 point) const
{
    return static_cast<size_t>(point.y) * m_mapSize.x + point.x;
}

yang::IVec2 DungeonMap::GetGridPointFromIndex(size_t index) const
{
    assert(m_mapSize.x != 0 && m_mapSize.y != 0);
    return yang::IVec2(static_cast<int>(index) % m_mapSize.x, static_cast<int>(index) / m_mapSize.x);
}

void DungeonMap::PlaceDoors()
{
    for (size_t i = 0; i < m_map.size(); ++i)
    {
        if (CheckShape(i))
        {
            SetTile(i, TileType::kDoor);
        }
    }
}

void DungeonMap::GenerateRoomGraph()
{
    size_t startIndex = GetIndexFromGridPoint(m_startPos);
    std::unordered_set<size_t> discoveredTiles;
    std::queue<size_t> roomOpenSet;
    std::queue<size_t> tileIndicesOpenSet;

    size_t prevRoomIndex = static_cast<size_t>(-1);
    size_t currentRoomColorIndex = 0;
    roomOpenSet.push(startIndex);

    // room bfs
    while (!roomOpenSet.empty())
    {
        startIndex = roomOpenSet.front();
        roomOpenSet.pop();

        if (discoveredTiles.count(startIndex))
        {
            size_t roomIndex = m_roomGraph.FindRoomIndex(startIndex);

            if (roomIndex != static_cast<size_t>(-1) && roomIndex != prevRoomIndex)
            {
                m_roomGraph.LinkRooms(prevRoomIndex, roomIndex);
            }
            continue;
        }

        size_t roomIndex = m_roomGraph.AddNewRoom();

        if (m_map[startIndex].m_tileType != TileType::kDoor)
        {
            m_roomGraph.AddTileToRoom(roomIndex, startIndex);
        }

        if (prevRoomIndex != static_cast<size_t>(-1))
        {
            m_roomGraph.LinkRooms(prevRoomIndex, roomIndex);
        }

        tileIndicesOpenSet.push(startIndex);
        discoveredTiles.emplace(startIndex);

        // Tile BFS
        while (!tileIndicesOpenSet.empty())
        {
            size_t tileIndex = tileIndicesOpenSet.front();
            tileIndicesOpenSet.pop();

            if (m_map[tileIndex].m_tileType == TileType::kDoor)
            {
                for (auto maybeIndex : GetAdjacentTiles(tileIndex))
                {
                    if (maybeIndex && m_map[*maybeIndex].m_tileType != TileType::kWall && discoveredTiles.count(*maybeIndex) == 0)
                    {
                        roomOpenSet.push(*maybeIndex);
                        break;
                    }
                }
                continue;
            }

            m_roomGraph.AddTileToRoom(roomIndex, tileIndex);

            for (auto maybeIndex : GetAdjacentTiles(tileIndex))
            {
                if (maybeIndex && m_map[*maybeIndex].m_tileType != TileType::kWall && discoveredTiles.count(*maybeIndex) == 0)
                {
                    discoveredTiles.emplace(*maybeIndex);
                    tileIndicesOpenSet.push(*maybeIndex);
                }
            }
        }

        prevRoomIndex = roomIndex;
    }
}

std::array<std::optional<size_t>, 4> DungeonMap::GetAdjacentTiles(size_t tileIndex) const
{
    std::array<std::optional<size_t>, 4> adjacentTiles;
    size_t currentIndex = 0;

    auto center = GetGridPointFromIndex(tileIndex);

    if (center.y - 1 < 0)
        adjacentTiles[0] = {};
    else
        adjacentTiles[0] = tileIndex - m_mapSize.x;

    if (center.x - 1 < 0)
        adjacentTiles[1] = {};
    else
        adjacentTiles[1] = tileIndex - 1;

    if (center.x + 1 >= m_mapSize.x)
        adjacentTiles[2] = {};
    else
        adjacentTiles[2] = tileIndex + 1;

    if (center.y + 1 >= m_mapSize.y)
        adjacentTiles[3] = {};
    else
        adjacentTiles[3] = tileIndex + m_mapSize.x;

    return adjacentTiles;
}

bool DungeonMap::CheckShape(size_t tileIndex)
{
    if (m_map[tileIndex].m_tileType != TileType::kTunnel)
        return false;

    auto center = GetGridPointFromIndex(tileIndex);
    
    // Is on border?
    if (center.x == 0 || center.x == m_mapSize.x - 1 || center.y == 0 || center.y == m_mapSize.y - 1)
        return false;

    bool fitsLeft = true;
    bool fitsTop = true;
    bool fitsRight = true;
    bool fitsDown = true;

    // Maybe the thing below can be done smarter, but no time to think haha

    // top left
    switch (m_map[tileIndex - m_mapSize.x - 1].m_tileType)
    {
    case TileType::kTunnel:
    {
        fitsLeft = fitsLeft && true;
        fitsTop = fitsTop && true;
        fitsRight = false;
        fitsDown = false;
        break;
    }
    case TileType::kWall:
    {
        fitsLeft = false;
        fitsTop = false;
        fitsRight = fitsRight && true;
        fitsDown = fitsDown && true;
        break;
    }
    }

    // top
    switch (m_map[tileIndex - m_mapSize.x].m_tileType)
    {
    case TileType::kTunnel:
    {
        fitsLeft = false;
        fitsTop = fitsTop && true;
        fitsRight = false;
        fitsDown = fitsDown && true;
        break;
    }
    case TileType::kWall:
    {
        fitsLeft = fitsLeft && true;
        fitsTop = false;
        fitsRight = fitsRight && true;
        fitsDown = false;
        break;
    }
    }

    // top right
    switch (m_map[tileIndex - m_mapSize.x + 1].m_tileType)
    {
    case TileType::kTunnel:
    {
        fitsLeft = false;
        fitsTop = fitsTop && true;
        fitsRight = fitsRight && true;
        fitsDown = false;
        break;
    }
    case TileType::kWall:
    {
        fitsLeft = fitsLeft && true;
        fitsTop = false;
        fitsRight = false;
        fitsDown = fitsDown && true;
        break;
    }
    }

    // left
    switch (m_map[tileIndex - 1].m_tileType)
    {
    case TileType::kTunnel:
    {
        fitsLeft = fitsLeft && true;
        fitsTop = false;
        fitsRight = fitsRight && true;
        fitsDown = false;
        break;
    }
    case TileType::kWall:
    {
        fitsLeft = false;
        fitsTop = fitsTop && true;
        fitsRight = false;
        fitsDown = fitsDown && true;
        break;
    }
    }

    // right
    switch (m_map[tileIndex + 1].m_tileType)
    {
    case TileType::kTunnel:
    {
        fitsLeft = fitsLeft && true;
        fitsTop = false;
        fitsRight = fitsRight && true;
        fitsDown = false;
        break;
    }
    case TileType::kWall:
    {
        fitsLeft = false;
        fitsTop = fitsTop && true;
        fitsRight = false;
        fitsDown = fitsDown && true;
        break;
    }
    }

    // down left
    switch (m_map[tileIndex + m_mapSize.x - 1].m_tileType)
    {
    case TileType::kTunnel:
    {
        fitsLeft = fitsLeft && true;
        fitsTop = false;
        fitsRight = false;
        fitsDown = fitsDown && true;
        break;
    }
    case TileType::kWall:
    {
        fitsLeft = false;
        fitsTop = fitsTop && true;
        fitsRight = fitsRight && true;
        fitsDown = false;
        break;
    }
    }

    // down
    switch (m_map[tileIndex + m_mapSize.x].m_tileType)
    {
    case TileType::kTunnel:
    {
        fitsLeft = false;
        fitsTop = fitsTop && true;
        fitsRight = false;
        fitsDown = fitsDown && true;
        break;
    }
    case TileType::kWall:
    {
        fitsLeft = fitsLeft && true;
        fitsTop = false;
        fitsRight = fitsRight && true;
        fitsDown = false;
        break;
    }
    }

    // down right
    switch (m_map[tileIndex + m_mapSize.x + 1].m_tileType)
    {
    case TileType::kTunnel:
    {
        fitsLeft = false;
        fitsTop = false;
        fitsRight = fitsRight && true;
        fitsDown = fitsDown && true;
        break;
    }
    case TileType::kWall:
    {
        fitsLeft = fitsLeft && true;
        fitsTop = fitsTop && true;
        fitsRight = false;
        fitsDown = false;
        break;
    }
    }

    return fitsLeft || fitsTop || fitsDown || fitsRight;
}

size_t DungeonMap::RoomGraph::FindRoomIndex(size_t tileIndex)
{
    for (size_t i = 0; i < m_rooms.size(); ++i)
    {
        if (m_rooms[i].m_tileIndices.count(tileIndex))
        {
            return i;
        }
    }
    return static_cast<size_t>(-1);
}

void DungeonMap::RoomGraph::LinkRooms(size_t first, size_t second)
{
    m_connections.push_back({ first, second });
}

size_t DungeonMap::RoomGraph::AddNewRoom()
{
    auto& room = m_rooms.emplace_back();
    room.m_colorIndex = m_currentColorIndex++;
    return m_rooms.size() - 1;
}

void DungeonMap::RoomGraph::AddTileToRoom(size_t roomIndex, size_t tileIndex)
{
    auto& roomTiles = m_rooms[roomIndex].m_tileIndices;
    if (roomTiles.count(tileIndex) == 0)
    {
        roomTiles.emplace(tileIndex);
    }
}

