#include "DungeonMap.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Utils/XMLHelpers.h>
#include <Utils/Rectangle.h>
#include <cassert>
#include <algorithm>

DungeonMap::DungeonMap(yang::IVec2 mapSize)
    :m_mapSize(mapSize)
{
}

bool DungeonMap::Init(yang::IVec2 mapSize, yang::IVec2 tileSize)
{
    m_mapSize = mapSize;
    m_tileSize = tileSize;
    m_map = std::vector<TileType>(m_mapSize.x * m_mapSize.y, TileType::kWall);

    // TODO: Init from XML
    m_tileset[IndexFromTileType(TileType::kWall)] = yang::IColor(0x000000ff);
    m_tileset[IndexFromTileType(TileType::kTunnel)] = yang::IColor(127,127,127,255);
    m_tileset[IndexFromTileType(TileType::kPlayer)] = yang::IColor(200,50,50,255);

    return true;
}

void DungeonMap::SetTile(yang::IVec2 position, TileType tileType)
{
    m_map[GetIndexFromGridPoint(position)] = tileType;
    //m_dirtyFlag = true;
}

void DungeonMap::PlaceRoom(yang::IVec2 center, yang::IVec2 dimensions)
{
    int startX = std::clamp(center.x - dimensions.x / 2, 1, m_mapSize.x - 2);
    int startY = std::clamp(center.y - dimensions.y / 2, 1, m_mapSize.y - 2);
    int endX = std::clamp(center.x + dimensions.x / 2 + dimensions.x % 2, 1, m_mapSize.x - 2);
    int endY = std::clamp(center.y + dimensions.y / 2 + dimensions.y % 2, 1, m_mapSize.y - 2);


    for (int y = startY; y != endY; ++y)
    {
        std::fill(m_map.begin() + GetIndexFromGridPoint({ startX, y }), m_map.begin() + GetIndexFromGridPoint({ endX,y }), TileType::kTunnel);
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
        success = pGraphics->FillRect(target, m_tileset[static_cast<size_t>(m_map[i])]);
    }
    return success;
}

void DungeonMap::Update(float deltaSeconds)
{
}

void DungeonMap::Clear()
{
    std::fill(m_map.begin(), m_map.end(), TileType::kWall);
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
