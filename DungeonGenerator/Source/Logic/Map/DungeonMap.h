#pragma once
#include <Utils/Vector2.h>
#include <Utils/Color.h>
#include <Application/Graphics/IGraphics.h>
#include <vector>
#include <string_view>
#include <array>

class DungeonMap
{
public:
    enum class TileType
    {
        kTunnel,
        kWall,
        kPlayer,
        kDoor,
        kMaxTypes
    };
public:
    DungeonMap() = default;
    DungeonMap(yang::IVec2 mapSize);

    bool Init(yang::IVec2 mapSize, yang::IVec2 tileSize);

    void SetTile(yang::IVec2 position, TileType tileType);
    void PlaceRoom(yang::IVec2 center, yang::IVec2 dimensions);

    bool Render(yang::IGraphics* pGraphics) const;
    void Update(float deltaSeconds);

    void Clear();

    size_t GetIndexFromGridPoint(yang::IVec2 point) const;
    yang::IVec2 GetGridPointFromIndex(size_t index) const;

    void PlaceDoors();

    static constexpr size_t IndexFromTileType(TileType t) { return static_cast<size_t>(t); }
    static constexpr TileType TileTypeFromIndex(size_t index) { return static_cast<TileType>(index); }
private:
    bool CheckShape(size_t tileIndex);
private:
    yang::IVec2 m_mapSize;
    yang::IVec2 m_tileSize;

    std::vector<TileType> m_map;
    std::array<yang::IColor, static_cast<size_t>(TileType::kMaxTypes)> m_tileset;
};

