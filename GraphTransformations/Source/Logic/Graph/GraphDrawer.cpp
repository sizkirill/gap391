#include "GraphDrawer.h"
#include "GraphCommons.h"
#include <Application/Graphics/IGraphics.h>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <Utils/Math.h>
#include "Graph.h"

#include <Application/Resources/ResourceCache.h>

GraphDrawer::GraphDrawer()
{
    m_pFont = yang::ResourceCache::Get()->Load<yang::IFont>("Assets/Fonts/Goldman-Regular.ttf", 20);
}

void GraphDrawer::BuildTileMap(const Graph& graph)
{
    std::unordered_map<NodeIndex, int> nodeToIndicesMap;

    graph.ForEachNode([this, &nodeToIndicesMap, &graph](const Node& node)
        {
            nodeToIndicesMap.emplace(node.GetIndex(), m_drawInfos.size());
            auto& drawInfo = m_drawInfos.emplace_back();
            drawInfo.m_nodeType = node.GetNodeType();
            drawInfo.m_degree = graph.GetDegree(node.GetIndex());
        });

    graph.ForEachEdge([this, &nodeToIndicesMap, &graph](NodeIndex from, NodeIndex to)
        {
            int fromIndex = nodeToIndicesMap[from];
            int toIndex = nodeToIndicesMap[to];
            m_drawInfos[fromIndex].m_neighborIndices.push_back(toIndex);
        });

    nodeToIndicesMap.clear();

    std::unordered_set<int> discoveredSet;
    std::queue<int> indicesQueue;
    m_drawInfos[0].m_nodeCenter = { 120,120 };
    discoveredSet.emplace(0);
    indicesQueue.emplace(0);

    while (!indicesQueue.empty())
    {
        int currentNode = indicesQueue.front();
        indicesQueue.pop();

        if (m_drawInfos[currentNode].m_neighborIndices.size() == 0)
            continue;

        float angleStep = (Math::kPi) / m_drawInfos[currentNode].m_degree;
        float currentAngle = 0;

        for (auto connectionIndex : m_drawInfos[currentNode].m_neighborIndices)
        {
            if (discoveredSet.count(connectionIndex) == 0)
            {
                discoveredSet.emplace(connectionIndex);
                indicesQueue.emplace(connectionIndex);
                m_drawInfos[connectionIndex].m_nodeCenter = m_drawInfos[currentNode].m_nodeCenter + yang::FVec2::FromAngle(currentAngle) * (float)DrawInfo::kRadius * 4.f;
                currentAngle += angleStep;
            }
        }
    }
}

void GraphDrawer::Finalize()
{
}

void GraphDrawer::Render(yang::IGraphics* pGraphics)
{
    for (auto& drawInfo : m_drawInfos)
    {
        for (auto index : drawInfo.m_neighborIndices)
        {
            pGraphics->DrawLine(drawInfo.m_nodeCenter, m_drawInfos[index].m_nodeCenter, yang::IColor::kWhite);
        }

        pGraphics->FillCircle(drawInfo.m_nodeCenter, DrawInfo::kRadius, ColorFromNodeType(drawInfo.m_nodeType));

        yang::FRect dst;
        dst.x = drawInfo.m_nodeCenter.x - DrawInfo::kRadius / 2;
        dst.y = drawInfo.m_nodeCenter.y - DrawInfo::kRadius / 2;
        dst.width = DrawInfo::kRadius;
        dst.height = DrawInfo::kRadius;

        pGraphics->DrawSprite(SpriteFromNodeType(drawInfo.m_nodeType, m_pFont), dst);
    }
}

void GraphDrawer::Reset()
{
    m_drawInfos.clear();
}
