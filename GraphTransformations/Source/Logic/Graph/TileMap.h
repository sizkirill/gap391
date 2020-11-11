#pragma once
#include <unordered_map>
#include "GraphCommons.h"
#include <Utils/Vector2.h>
#include <array>
#include <memory>

class Graph;

namespace yang
{
    class IGraphics;
    class IFont;
}

class TileMap
{
public:
    enum class Direction
    {
        kRight,
        kDown,
        kLeft,
        kTop,
        kInvalidDirection
    };

    static constexpr yang::IVec2 kDimensions = { 10,10 };
    static constexpr yang::IVec2 kTileSize = { 72,72 };
    
    void BuildTileMap(const Graph& graph);
    void Finalize();
    void Render(yang::IGraphics* pGraphics);
    void Reset();
private:
    std::unordered_map<int, NodeType> m_tileIndexToNodeType;
    std::unordered_map<int, std::array<int, 4>> m_connectedTiles;

    int m_minIndex;
    std::shared_ptr<yang::IFont> m_pFont;

    int GetNeighbor(int fromIndex, Direction dir) const;
    bool HasNeighbor(int fromIndex, Direction dir) const;
    int GetEmptyNeighborIndex(int fromIndex) const;
    Direction GetDirection(int fromIndex, int toIndex) const;
    bool CheckConnection(int from, int to) const;
};