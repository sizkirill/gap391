#pragma once
#include <Utils/Vector2.h>
#include <Utils/Color.h>
#include <Application/Graphics/IGraphics.h>
#include <vector>
#include <string_view>
#include <array>
#include <unordered_set>
#include <optional>

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

    struct Tile
    {
        TileType m_tileType;
        yang::IColor m_color;
    };

    struct Room
    {
        std::unordered_set<size_t> m_tileIndices;
        size_t m_colorIndex;
    };

    struct RoomConnection
    {
        size_t m_firstRoomIndex;
        size_t m_secondRoomIndex;
    };

    struct RoomGraph
    {
        std::vector<Room> m_rooms;
        std::vector<RoomConnection> m_connections;
        size_t m_currentColorIndex = 0;

        size_t FindRoomIndex(size_t tileIndex);
        void LinkRooms(size_t first, size_t second);
        size_t AddNewRoom();
        void AddTileToRoom(size_t roomIndex, size_t tileIndex);
    };
public:
    DungeonMap() = default;
    DungeonMap(yang::IVec2 mapSize);

    bool Init(yang::IVec2 mapSize, yang::IVec2 tileSize, yang::IVec2 startPos);

    void SetTile(yang::IVec2 position, TileType tileType);
    void SetTile(size_t index, TileType tileType);
    void PlaceRoom(yang::IVec2 center, yang::IVec2 dimensions);

    bool Render(yang::IGraphics* pGraphics) const;
    void Update(float deltaSeconds);

    size_t GetIndexFromGridPoint(yang::IVec2 point) const;
    yang::IVec2 GetGridPointFromIndex(size_t index) const;

    void PlaceDoors();
    void GenerateRoomGraph();

    std::array<std::optional<size_t>, 4> GetAdjacentTiles(size_t tileIndex) const;

    static constexpr size_t IndexFromTileType(TileType t) { return static_cast<size_t>(t); }
    static constexpr TileType TileTypeFromIndex(size_t index) { return static_cast<TileType>(index); }
private:
    bool CheckShape(size_t tileIndex);
private:
    yang::IVec2 m_mapSize;
    yang::IVec2 m_tileSize;

    yang::IVec2 m_startPos;

    std::vector<Tile> m_map;
    RoomGraph m_roomGraph;
    std::array<yang::IColor, static_cast<size_t>(TileType::kMaxTypes)> m_tileset;
    std::array<yang::IColor, 10> m_roomColors{0x800000ff, 0x008000ff, 0x000080ff, 0x800080ff, 0x008080ff, 0x00ffffff, 0xffffffff, 0x808000ff, 0x00ff00ff, 0x0000ffff};
};

