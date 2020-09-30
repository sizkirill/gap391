#pragma once
#include <Utils/Vector2.h>
#include <Utils/Color.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/Textures/Sprite.h>
#include <vector>
#include <string_view>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <optional>
#include <memory>

class BspTreeNode;
class DungeonMap;

class RoomGraph
{
public:
private:
    struct RoomNode
    {
        RoomNode(size_t id) : m_id(id) {}

        size_t m_id = std::numeric_limits<size_t>::max();
        std::vector<size_t> m_tileIndices;
        //RoomType
    };

    std::vector<RoomNode> m_roomNodes;
    std::vector<std::unordered_set<size_t>> m_adjacencyList;
    std::unordered_map<size_t, size_t> m_tileToRoomMap;
    DungeonMap* m_pOwner;
    // player path
public:
    RoomGraph(DungeonMap* pOwner) : m_pOwner(pOwner) {}

    size_t AddEmptyNode();
    void LinkNodes(size_t from, size_t to);
    void AddTileToRoom(size_t tileIndex, size_t roomIndex);
    void BuildRooms(BspTreeNode* pNode);
    void GenerateRoom(BspTreeNode* pLeaf);
    void GenerateCorridors(BspTreeNode* pRoot);
    void BuildCorridor(BspTreeNode* pSource, BspTreeNode* pDest);
};

class DungeonMap
{
public:
    enum class TileType
    {
        kTunnel,
        kTopLeftWall,
        kTopWall,
        kTopRightWall,
        kLeftWall,
        kRoom,
        kRightWall,
        kDownLeftWall,
        kDownWall,
        kDownRightWall,
        kNothing,
        kPlayer,
        kDoor,
        kMaxTypes
    };

    struct Tile
    {
        TileType m_tileType;
        std::shared_ptr<yang::Sprite> m_pSprite;
    };

    //struct Room
    //{
    //    std::unordered_set<size_t> m_tileIndices;
    //    size_t m_colorIndex;
    //};

    //struct RoomConnection
    //{
    //    size_t m_firstRoomIndex;
    //    size_t m_secondRoomIndex;
    //};

    //struct RoomGraph
    //{
    //    std::vector<Room> m_rooms;
    //    std::vector<RoomConnection> m_connections;
    //    size_t m_currentColorIndex = 0;

    //    size_t FindRoomIndex(size_t tileIndex);
    //    void LinkRooms(size_t first, size_t second);
    //    size_t AddNewRoom();
    //    void AddTileToRoom(size_t roomIndex, size_t tileIndex);
    //};

    std::shared_ptr<BspTreeNode> m_pMapTree;
    std::vector<BspTreeNode*> m_pLeaves;
public:
    DungeonMap() = default;
    DungeonMap(yang::IVec2 mapSize);

    bool Init(yang::IVec2 mapSize, yang::IVec2 tileSize, yang::IVec2 startPos, const std::unordered_map<uint32_t, std::shared_ptr<yang::Sprite>>& spriteMap);

    void SetTile(yang::IVec2 position, TileType tileType);
    void SetTile(size_t index, TileType tileType);
    void PlaceRoom(yang::IVec2 center, yang::IVec2 dimensions);

    bool Render(yang::IGraphics* pGraphics) const;
    void Update(float deltaSeconds);

    size_t GetIndexFromGridPoint(yang::IVec2 point) const;
    yang::IVec2 GetGridPointFromIndex(size_t index) const;
    bool IsAngleTile(size_t index) const;

    void PlaceDoors();
    void GenerateRoomGraph(BspTreeNode* pRoot);

    std::array<std::optional<size_t>, 4> GetAdjacentTiles(size_t tileIndex) const;

    static constexpr size_t IndexFromTileType(TileType t) { return static_cast<size_t>(t); }
    static constexpr TileType TileTypeFromIndex(size_t index) { return static_cast<TileType>(index); }

    // TODO: remove
    void SetMapTree(std::shared_ptr<BspTreeNode> pTree) { m_pMapTree = pTree; }
    void SetLeaves(std::vector<BspTreeNode*>&& leaves) { m_pLeaves = std::move(leaves); }
    ~DungeonMap();
private:
    yang::IVec2 m_mapSize;
    yang::IVec2 m_tileSize;

    yang::IVec2 m_startPos;

    std::vector<Tile> m_map;
    RoomGraph m_roomGraph = RoomGraph(this);
    std::array<std::shared_ptr<yang::Sprite>, static_cast<size_t>(TileType::kMaxTypes)> m_tileset;
};

