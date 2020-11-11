#pragma once
#include <Utils/Vector2.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <Utils/Color.h>
#include <Logic/Graph/GraphCommons.h>

class Graph;

namespace yang
{
    class IGraphics;
    class IFont;
}

class GraphDrawer
{
public:

    struct DrawInfo
    {
        static constexpr int kRadius = 20;
        yang::FVec2 m_nodeCenter;
        NodeType m_nodeType;
        std::vector<size_t> m_neighborIndices;
        size_t m_degree;
    };

    GraphDrawer();
    void BuildTileMap(const Graph& graph);
    void Finalize();
    void Render(yang::IGraphics* pGraphics);
    void Reset();
private:
    std::vector<DrawInfo> m_drawInfos;

    std::shared_ptr<yang::IFont> m_pFont;
};