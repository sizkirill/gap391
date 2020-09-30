#include "DungeonMap.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Utils/XMLHelpers.h>
#include <Utils/Rectangle.h>
#include <unordered_set>
#include <queue>
#include <cassert>
#include <algorithm>
#include <Logic/Map/BspTree.h>
#include <Utils/StringHash.h>

DungeonMap::DungeonMap(yang::IVec2 mapSize)
    :m_mapSize(mapSize)
    ,m_roomGraph(this)
{
}

bool DungeonMap::Init(yang::IVec2 mapSize, yang::IVec2 tileSize, yang::IVec2 startPos, const std::unordered_map<uint32_t, std::shared_ptr<yang::Sprite>>& spriteMap)
{
    m_mapSize = mapSize;
    m_tileSize = tileSize;
    m_startPos = startPos;

    auto spriteLambda = [](auto& map, std::string_view spriteName, uint32_t defaultVal)
    {
        uint32_t hashVal = StringHash32(spriteName.data());
        if (map.find(hashVal) == map.end())
            return map.at(defaultVal);

        return map.at(hashVal);
    };

    uint32_t defaultHashVal = StringHash32("nothing");

    if (spriteMap.find(defaultHashVal) == spriteMap.end())
        return false;

    m_tileset[IndexFromTileType(TileType::kTunnel)] = spriteLambda(spriteMap, "room", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kTopLeftWall)] = spriteLambda(spriteMap, "topLeftWall", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kTopWall)] = spriteLambda(spriteMap, "topWall", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kTopRightWall)] = spriteLambda(spriteMap, "topRightWall", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kLeftWall)] = spriteLambda(spriteMap, "leftWall", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kRoom)] = spriteLambda(spriteMap, "room", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kRightWall)] = spriteLambda(spriteMap, "rightWall", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kDownLeftWall)] = spriteLambda(spriteMap, "downLeftWall", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kDownWall)] = spriteLambda(spriteMap, "downWall", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kDownRightWall)] = spriteLambda(spriteMap, "downRightWall", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kNothing)] = spriteLambda(spriteMap, "nothing", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kPlayer)] = spriteLambda(spriteMap, "player", defaultHashVal);
    m_tileset[IndexFromTileType(TileType::kDoor)] = spriteLambda(spriteMap, "room", defaultHashVal);

    Tile nothing{ TileType::kNothing, m_tileset[static_cast<size_t>(TileType::kNothing)] };
    m_map = std::vector<Tile>(m_mapSize.x * m_mapSize.y, nothing);

    return true;
}

void DungeonMap::SetTile(yang::IVec2 position, TileType tileType)
{
    m_map[GetIndexFromGridPoint(position)].m_tileType = tileType;
    m_map[GetIndexFromGridPoint(position)].m_pSprite = m_tileset[static_cast<size_t>(tileType)];
}

void DungeonMap::SetTile(size_t index, TileType tileType)
{
    m_map[index].m_tileType = tileType;
    m_map[index].m_pSprite = m_tileset[static_cast<size_t>(tileType)];
}

void DungeonMap::PlaceRoom(yang::IVec2 center, yang::IVec2 dimensions)
{
    //int startX = std::clamp(center.x - dimensions.x / 2, 1, m_mapSize.x - 2);
    //int startY = std::clamp(center.y - dimensions.y / 2, 1, m_mapSize.y - 2);
    //int endX = std::clamp(center.x + dimensions.x / 2 + dimensions.x % 2, 1, m_mapSize.x - 2);
    //int endY = std::clamp(center.y + dimensions.y / 2 + dimensions.y % 2, 1, m_mapSize.y - 2);

    //Tile room{ TileType::kTunnel, m_tileset[static_cast<size_t>(TileType::kTunnel)] };
    //for (int y = startY; y != endY; ++y)
    //{
    //    std::fill(m_map.begin() + GetIndexFromGridPoint({ startX, y }), m_map.begin() + GetIndexFromGridPoint({ endX,y }), room);
    //}
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
        success = pGraphics->DrawSprite(m_map[i].m_pSprite, target);
    }

    //for (auto& room : m_roomGraph.m_rooms)
    //{
    //    size_t roomColorIndex = room.m_colorIndex;

    //    for (auto& index : room.m_tileIndices)
    //    {
    //        auto point = GetGridPointFromIndex(index);
    //        yang::IRect target;
    //        target.x = point.x * m_tileSize.x + m_tileSize.x / 4;
    //        target.y = point.y * m_tileSize.y + m_tileSize.y / 4;
    //        target.width = m_tileSize.x - m_tileSize.x / 2;
    //        target.height = m_tileSize.y - m_tileSize.y / 2;
    //        success = pGraphics->FillRect(target, m_roomColors[roomColorIndex % m_roomColors.size()]);
    //    }
    //}
    //return success;

    //m_pMapTree->ForEach([this, pGraphics](BspTreeNode* pNode)
    //    {
    //        yang::IRect dest = pNode->m_areaRect;
    //        dest.x *= m_tileSize.x;
    //        dest.y *= m_tileSize.y;
    //        dest.width *= m_tileSize.x;
    //        dest.height *= m_tileSize.y;

    //        pGraphics->DrawRect(dest, yang::IColor::kWhite);
    //    });

    //for (auto pLeave : m_pLeaves)
    //{
    //    yang::IRect dest = pLeave->m_areaRect;
    //    dest.x *= m_tileSize.x;
    //    dest.y *= m_tileSize.y;
    //    dest.width *= m_tileSize.x;
    //    dest.height *= m_tileSize.y;

    //    pGraphics->DrawRect(dest, yang::IColor::kWhite);
    //}

    return true;
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

bool DungeonMap::IsAngleTile(size_t index) const
{
    return (m_map[index].m_tileType == TileType::kDownLeftWall ||
            m_map[index].m_tileType == TileType::kTopLeftWall ||
            m_map[index].m_tileType == TileType::kTopRightWall ||
            m_map[index].m_tileType == TileType::kDownRightWall);

}   

void DungeonMap::PlaceDoors()
{
    //for (size_t i = 0; i < m_map.size(); ++i)
    //{
    //    if (CheckShape(i))
    //    {
    //        SetTile(i, TileType::kDoor);
    //    }
    //}
}

void DungeonMap::GenerateRoomGraph(BspTreeNode* pRoot)
{
    m_roomGraph.BuildRooms(pRoot);
    m_roomGraph.GenerateCorridors(pRoot);

    //size_t startIndex = GetIndexFromGridPoint(m_startPos);
    //std::unordered_set<size_t> discoveredTiles;
    //std::queue<size_t> roomOpenSet;
    //std::queue<size_t> tileIndicesOpenSet;

    //size_t prevRoomIndex = static_cast<size_t>(-1);
    //size_t currentRoomColorIndex = 0;
    //roomOpenSet.push(startIndex);

    //// room bfs
    //while (!roomOpenSet.empty())
    //{
    //    startIndex = roomOpenSet.front();
    //    roomOpenSet.pop();

    //    if (discoveredTiles.count(startIndex))
    //    {
    //        size_t roomIndex = m_roomGraph.FindRoomIndex(startIndex);

    //        if (roomIndex != static_cast<size_t>(-1) && roomIndex != prevRoomIndex)
    //        {
    //            m_roomGraph.LinkRooms(prevRoomIndex, roomIndex);
    //        }
    //        continue;
    //    }

    //    size_t roomIndex = m_roomGraph.AddNewRoom();

    //    if (m_map[startIndex].m_tileType != TileType::kDoor)
    //    {
    //        m_roomGraph.AddTileToRoom(roomIndex, startIndex);
    //    }

    //    if (prevRoomIndex != static_cast<size_t>(-1))
    //    {
    //        m_roomGraph.LinkRooms(prevRoomIndex, roomIndex);
    //    }

    //    tileIndicesOpenSet.push(startIndex);
    //    discoveredTiles.emplace(startIndex);

    //    // Tile BFS
    //    while (!tileIndicesOpenSet.empty())
    //    {
    //        size_t tileIndex = tileIndicesOpenSet.front();
    //        tileIndicesOpenSet.pop();

    //        if (m_map[tileIndex].m_tileType == TileType::kDoor)
    //        {
    //            for (auto maybeIndex : GetAdjacentTiles(tileIndex))
    //            {
    //                if (maybeIndex && m_map[*maybeIndex].m_tileType != TileType::kWall && discoveredTiles.count(*maybeIndex) == 0)
    //                {
    //                    roomOpenSet.push(*maybeIndex);
    //                    break;
    //                }
    //            }
    //            continue;
    //        }

    //        m_roomGraph.AddTileToRoom(roomIndex, tileIndex);

    //        for (auto maybeIndex : GetAdjacentTiles(tileIndex))
    //        {
    //            if (maybeIndex && m_map[*maybeIndex].m_tileType != TileType::kWall && discoveredTiles.count(*maybeIndex) == 0)
    //            {
    //                discoveredTiles.emplace(*maybeIndex);
    //                tileIndicesOpenSet.push(*maybeIndex);
    //            }
    //        }
    //    }

    //    prevRoomIndex = roomIndex;
    //}
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

DungeonMap::~DungeonMap()
{
}

//size_t DungeonMap::RoomGraph::FindRoomIndex(size_t tileIndex)
//{
//    for (size_t i = 0; i < m_rooms.size(); ++i)
//    {
//        if (m_rooms[i].m_tileIndices.count(tileIndex))
//        {
//            return i;
//        }
//    }
//    return static_cast<size_t>(-1);
//}
//
//void DungeonMap::RoomGraph::LinkRooms(size_t first, size_t second)
//{
//    m_connections.push_back({ first, second });
//}
//
//size_t DungeonMap::RoomGraph::AddNewRoom()
//{
//    auto& room = m_rooms.emplace_back();
//    room.m_colorIndex = m_currentColorIndex++;
//    return m_rooms.size() - 1;
//}
//
//void DungeonMap::RoomGraph::AddTileToRoom(size_t roomIndex, size_t tileIndex)
//{
//    auto& roomTiles = m_rooms[roomIndex].m_tileIndices;
//    if (roomTiles.count(tileIndex) == 0)
//    {
//        roomTiles.emplace(tileIndex);
//    }
//}

size_t RoomGraph::AddEmptyNode()
{
    size_t newIndex = m_roomNodes.size();
    m_roomNodes.emplace_back(newIndex);
    m_adjacencyList.emplace_back();
    return newIndex;
}

void RoomGraph::LinkNodes(size_t from, size_t to)
{
    m_adjacencyList[from].insert(to);
    m_adjacencyList[to].insert(from);
}

void RoomGraph::AddTileToRoom(size_t tileIndex, size_t roomIndex)
{
    m_roomNodes[roomIndex].m_tileIndices.push_back(tileIndex);
    m_tileToRoomMap[tileIndex] = roomIndex;
}

void RoomGraph::BuildRooms(BspTreeNode* pNode)
{
    if (!pNode)
        return;

    if (!pNode->m_pLeft && !pNode->m_pRight)
    {
        GenerateRoom(pNode);
        return;
    }

    BuildRooms(pNode->m_pLeft);
    BuildRooms(pNode->m_pRight);
}

void RoomGraph::GenerateRoom(BspTreeNode* pLeaf)
{
    size_t roomId = AddEmptyNode();

    const yang::IRect& areaRect = pLeaf->m_areaRect;

    for (int y = areaRect.y; y < areaRect.y + areaRect.height; ++y)
    {
        for (int x = areaRect.x; x < areaRect.x + areaRect.width; ++x)
        {
            if (y == areaRect.y && x == areaRect.x)
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kTopLeftWall);
            }

            else if (y == areaRect.y + areaRect.height - 1 && x == areaRect.x)
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kDownLeftWall);
            }

            else if (y == areaRect.y && x == areaRect.x + areaRect.width - 1)
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kTopRightWall);
            }

            else if (y == areaRect.y + areaRect.height - 1 && x == areaRect.x + areaRect.width - 1)
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kDownRightWall);
            }

            else if (y == areaRect.y)
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kTopWall);
            }

            else if (x == areaRect.x)
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kLeftWall);
            }

            else if (y == areaRect.y + areaRect.height - 1)
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kDownWall);
            }

            else if (x == areaRect.x + areaRect.width - 1)
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kRightWall);
            }

            else
            {
                m_pOwner->SetTile(m_pOwner->GetIndexFromGridPoint({ x,y }), DungeonMap::TileType::kRoom);
            }

            m_tileToRoomMap[m_pOwner->GetIndexFromGridPoint({ x,y })] = roomId;
        }
    }

}

void RoomGraph::GenerateCorridors(BspTreeNode* pRoot)
{
    std::vector<std::unordered_set<BspTreeNode*>> nodesByDepth = pRoot->GetNodesByDepth();

    for (auto rIt = nodesByDepth.rbegin(); rIt != nodesByDepth.rend(); ++rIt)
    {
        while (!rIt->empty())
        {
            auto nodeIt = rIt->begin();

            if (nodeIt == rIt->end() || *nodeIt == pRoot)
                break;

            auto sibling = (*nodeIt)->GetSibling();
            if (sibling)
            {
                BuildCorridor(*nodeIt, sibling);
                rIt->erase(sibling);
            }
            rIt->erase(nodeIt);
        }
    }
}

void RoomGraph::BuildCorridor(BspTreeNode* pSource, BspTreeNode* pDest)
{
    assert(pSource && pDest && pSource->m_pParent == pDest->m_pParent && pSource->m_pParent);

    auto doorPoints = pSource->m_pParent->GenerateDoorTileIndices();

    while (m_pOwner->IsAngleTile(m_pOwner->GetIndexFromGridPoint(doorPoints.first)) || m_pOwner->IsAngleTile(m_pOwner->GetIndexFromGridPoint(doorPoints.second)))
    {
        doorPoints = pSource->m_pParent->GenerateDoorTileIndices();
    }

    m_pOwner->SetTile(doorPoints.first, DungeonMap::TileType::kTunnel);
    m_pOwner->SetTile(doorPoints.second, DungeonMap::TileType::kTunnel);
}

