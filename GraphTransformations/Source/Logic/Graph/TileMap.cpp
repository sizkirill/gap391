#include "TileMap.h"
#include "Graph.h"
#include <Application/Graphics/IGraphics.h>
#include <Application/Resources/ResourceCache.h>
#include <Application/Graphics/Fonts/IFont.h>
#include <Utils/Rectangle.h>
#include <algorithm>

void TileMap::BuildTileMap(const Graph& graph)
{
    std::unordered_map<NodeIndex, int> nodeIndicesToMapIndices;

    graph.ForEachNodeAndEdge([this, &nodeIndicesToMapIndices](const Node& node) 
        {
            if (nodeIndicesToMapIndices.count(node.GetIndex()) == 0)
            {
                nodeIndicesToMapIndices.emplace(node.GetIndex(), nodeIndicesToMapIndices.size());
                m_tileIndexToNodeType.emplace(nodeIndicesToMapIndices.at(node.GetIndex()), node.GetNodeType());
            }
            else if (m_tileIndexToNodeType.count(nodeIndicesToMapIndices.at(node.GetIndex())) == 0)
            {
                m_tileIndexToNodeType.emplace(nodeIndicesToMapIndices.at(node.GetIndex()), node.GetNodeType());
            }
        },

        [this, &nodeIndicesToMapIndices, &graph](NodeIndex from, NodeIndex to) 
        {
            auto tileIndex = nodeIndicesToMapIndices.at(from);

            if (m_connectedTiles.count(tileIndex) == 0)
            {
                m_connectedTiles.emplace(tileIndex, std::array<int, 4>());
                m_connectedTiles.at(tileIndex).fill(yang::kInvalidValue<int>);
            }

            if (nodeIndicesToMapIndices.count(to) == 0)
            {
                int toIndex = GetEmptyNeighborIndex(tileIndex);
                if (toIndex == yang::kInvalidValue<int>)
                {
                    return;
                }
                nodeIndicesToMapIndices.emplace(to, toIndex);
            }

            int toIndex = nodeIndicesToMapIndices.at(to);

            Direction dir = GetDirection(tileIndex, toIndex);
            if (dir != Direction::kInvalidDirection)
            {
                m_connectedTiles.at(tileIndex)[static_cast<size_t>(dir)] = toIndex;
            }
            
        });
}

void TileMap::Finalize()
{
    auto minIndexPair = std::min_element(m_tileIndexToNodeType.begin(), m_tileIndexToNodeType.end(), [this](auto& first, auto& second) 
        {
            return first.first < second.first;
        }
    );

    m_pFont = yang::ResourceCache::Get()->Load<yang::IFont>("Assets/Fonts/Goldman-Bold.ttf", 30);

    m_minIndex = minIndexPair->first;
}

void TileMap::Render(yang::IGraphics* pGraphics)
{
    int amountDown = kDimensions.x * (std::abs(m_minIndex) / kDimensions.x);
    int amountRight = std::abs(m_minIndex) % kDimensions.x;

    for (auto [index, nodeType] : m_tileIndexToNodeType)
    {
        yang::IRect dst;
        int actualIndex = index + amountDown + amountRight;
        dst.x = (actualIndex % kDimensions.x) * kTileSize.x;
        dst.y = (actualIndex / kDimensions.x) * kTileSize.y;
        dst.width = kTileSize.x;
        dst.height = kTileSize.y;

        pGraphics->FillRect(dst, ColorFromNodeType(nodeType));
        pGraphics->DrawSprite(SpriteFromNodeType(nodeType, m_pFont), dst);
    }

    for (int i = 0; i <= kDimensions.x; ++i)
    {
        yang::IVec2 start{ i * kTileSize.x, 0 };
        yang::IVec2 end{ i * kTileSize.x, 720 };

        pGraphics->DrawLine(start, end, yang::IColor::kWhite);
    }

    for (int i = 0; i <= kDimensions.y; ++i)
    {
        yang::IVec2 start{0, i * kTileSize.y };
        yang::IVec2 end{720, i * kTileSize.y };

        pGraphics->DrawLine(start, end, yang::IColor::kWhite);
    }

    for (auto& [index, connections] : m_connectedTiles)
    {
        for (auto connectionIndex : connections)
        {
            int actualFromIndex = index + amountDown + amountRight;
            yang::IVec2 fromIndexCoords = { actualFromIndex % kDimensions.x, actualFromIndex / kDimensions.x };
            yang::FVec2 fromTilePos = yang::IVec2::Scale(fromIndexCoords, kTileSize);
            fromTilePos += kTileSize / 2;

            if (connectionIndex != yang::kInvalidValue<int>)
            {
                int actualToIndex = connectionIndex + amountDown + amountRight;

                yang::IVec2 toIndexCoords = { actualToIndex % kDimensions.x, actualToIndex / kDimensions.x };
                yang::FVec2 toTilePos = yang::IVec2::Scale(toIndexCoords, kTileSize);
                toTilePos += kTileSize / 2;
                
                yang::FVec2 doorPos = yang::FVec2::Lerp(fromTilePos, toTilePos, 0.5f);

                yang::FRect dst;
                dst.x = doorPos.x - kTileSize.x / 8.f;
                dst.y = doorPos.y - kTileSize.y / 8.f;
                dst.width = kTileSize.x / 4.f;
                dst.height = kTileSize.y / 4.f;

                pGraphics->FillRect(dst, 0x00ff0000);
            }
        }
    }
}

void TileMap::Reset()
{
    m_tileIndexToNodeType.clear();
    m_connectedTiles.clear();
    m_minIndex = 0;
}

int TileMap::GetNeighbor(int fromIndex, Direction dir) const
{
    switch (dir)
    {
    case Direction::kRight: return fromIndex + 1;
    case Direction::kDown: return fromIndex + kDimensions.x;
    case Direction::kLeft: return fromIndex - 1;
    case Direction::kTop: return fromIndex - kDimensions.x;
    default: return yang::kInvalidValue<int>;
    }
}

bool TileMap::HasNeighbor(int fromIndex, Direction dir) const
{
    auto toIndex = GetNeighbor(fromIndex, dir);
    if (toIndex != yang::kInvalidValue<int> && m_tileIndexToNodeType.count(toIndex) > 0)
    {
        return true;
    }

    return false;
}

int TileMap::GetEmptyNeighborIndex(int fromIndex) const
{
    for (int i = 0; i < static_cast<int>(Direction::kInvalidDirection); ++i)
    {
        auto toIndex = GetNeighbor(fromIndex, static_cast<Direction>(i));
        if (toIndex != yang::kInvalidValue<int> && m_tileIndexToNodeType.count(toIndex) == 0)
        {
            return toIndex;
        }
    }
    return yang::kInvalidValue<int>;
}

TileMap::Direction TileMap::GetDirection(int fromIndex, int toIndex) const
{
    int diff = toIndex - fromIndex;

    switch (diff)
    {
    case 1: return Direction::kRight;
    case -1: return Direction::kLeft;
    case kDimensions.x: return Direction::kDown;
    case -kDimensions.x: return Direction::kTop;
    default: return Direction::kInvalidDirection;
    }
}

bool TileMap::CheckConnection(int from, int to) const
{
    return GetDirection(from, to) != Direction::kInvalidDirection;
}
