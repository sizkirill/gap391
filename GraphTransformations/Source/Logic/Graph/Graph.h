#pragma once
#include <unordered_set>
#include <vector>
#include <type_traits>
#include <unordered_map>
#include <string_view>
#include <cassert>
#include "GraphCommons.h"
#include <Utils/Random.h>

class TransformationRule;

struct Node
{
    NodeId m_nodeId;
    NodeType m_nodeType;

    NodeIndex GetIndex() const;
    NodeRefCount GetRefCount() const;
    NodeId SetRefCount(NodeRefCount refCount);
    NodeType GetNodeType() const { return m_nodeType; }
    void IncrementRef();
    void DecrementRef();
};

class Graph
{
public:
    NodeId AddNode(NodeType type);
    void DeleteNode(NodeId id);
    void SpliceSubGraph(NodeId nodeIdToReplace, const Graph& subGraph);
    void SpliceSubGraph(std::vector<NodeIndex>& pattern, const TransformationRule& rule);

    std::vector<NodeIndex> FindNodeIndicesByType(NodeType type);

    static Graph BuildGraphFromXML(std::string_view pathToXml);

    template <class F>
    void ForEachNode(F&& doStuff) const;

    template <class F>
    void ForEachEdge(F&& doStuff) const;

    template <class F>
    void ForEachOutEdge(NodeIndex fromIndex, F&& doStuff) const;

    template <class F>
    void ForEachInEdge(NodeIndex toIndex, F&& doStuff) const;

    template <class NodeFunc, class EdgeFunc>
    void ForEachNodeAndOutgoingEdge(NodeFunc&& nodeFunc, EdgeFunc&& edgeFunc) const;

    template <class NodeFunc, class InEdgeFunc, class OutEdgeFunc>
    void ForEachNodeAndEdge(NodeFunc&& nodeFunc, InEdgeFunc&& incomingEdgeFunc, OutEdgeFunc&& outgoingEdgeFunc) const;

    template <class EdgeFunc, class DiscoverFunc>
    void RunBFS(NodeIndex startNode, EdgeFunc&& visitEdge, DiscoverFunc&& onDiscover) const;

    bool Transform(const TransformationRule& rule, yang::XorshiftRNG& rng);

    size_t GetInDegree(NodeIndex index) const;
    size_t GetOutDegree(NodeIndex index) const;
    size_t GetDegree(NodeIndex index) const;
    NodeType GetNodeType(NodeIndex index) const;

    size_t Size() const { return m_nodes.size(); }

private:
    std::vector<Node> m_nodes;
    std::vector<std::unordered_set<NodeIndex>> m_outgoingAdjacencyList;
    std::vector<std::unordered_set<NodeIndex>> m_incomingAdjacencyList;
    std::vector<NodeIndex> m_freeList;

    NodeIndex m_entrance;
    NodeIndex m_exit;

    Node& GetNode(NodeId id);
    NodeIndex GetIndex(NodeId id) const;
    NodeRefCount GetRefCount(NodeId id) const;
    NodeId SetRefCount(NodeId id, NodeRefCount refCount) const;
    NodeId GetFreeId();
    std::vector<NodeIndex> UnlinkIncomingNodes(NodeIndex index);
    std::vector<NodeIndex> UnlinkOutgoingNodes(NodeIndex index);
    std::unordered_map<NodeIndex, NodeIndex> CreateIslandFromSubGraph(const Graph& subGraph);

    using NodeSequence = std::vector<NodeIndex>;
    using NodeSequenceList = std::vector<NodeSequence>;
    using DiscoveredSet = std::unordered_set<NodeIndex>;

    NodeSequenceList FindValidNodePatterns(const Graph& pattern) const;
    void FindValidNodePatternsWithDepthLimitedSearch(NodeIndex rootNodeId, const Graph& pattern, NodeSequenceList& outNodes) const;
    void FindValidNodePatternsWithDepthLimitedSearchHelper(NodeIndex nodeId, DiscoveredSet& discovered, const Graph& pattern, size_t nodePatternIndex, NodeSequence& workingNodeSequence, size_t workingNodeSequenceIndex, NodeSequenceList& outNodes) const;


    void LinkNodes(NodeIndex fromId, NodeIndex toId);
    void UnlinkNodes(NodeIndex fromId, NodeIndex toId);
};

template<class F>
inline void Graph::ForEachNode(F&& doStuff) const
{
    static_assert(std::is_invocable_r_v<void, decltype(doStuff), const Node&>, "Function needs to have signature void(const Node&)");
    for (auto& node : m_nodes)
    {
        if (node.GetNodeType() == NodeType::kMaxTypes)
        {
            continue;
        }

        doStuff(node);
    }
}

template<class F>
inline void Graph::ForEachEdge(F&& doStuff) const
{
    static_assert(std::is_invocable_r_v<void, decltype(doStuff), NodeIndex, NodeIndex>, "Function needs to have signature void(NodeIndex from, NodeIndex to)");
    for(size_t fromIndex = 0; fromIndex < m_outgoingAdjacencyList.size(); ++fromIndex)
    {
        if (m_nodes[fromIndex].GetNodeType() == NodeType::kMaxTypes)
        {
            continue;
        }

        for (auto toIndex : m_outgoingAdjacencyList[fromIndex])
        {
            if (m_nodes[toIndex].GetNodeType() == NodeType::kMaxTypes)
            {
                continue;
            }

            doStuff(fromIndex, toIndex);
        }
    }
}

template<class F>
inline void Graph::ForEachOutEdge(NodeIndex fromIndex, F&& doStuff) const
{
    static_assert(std::is_invocable_r_v<void, decltype(doStuff), NodeIndex, NodeIndex>, "Function needs to have signature void(NodeIndex from, NodeIndex to)");
    assert(fromIndex < m_outgoingAdjacencyList.size());
    for (auto toIndex : m_outgoingAdjacencyList[fromIndex])
    {
        doStuff(fromIndex, toIndex);
    }
}

template<class F>
inline void Graph::ForEachInEdge(NodeIndex toIndex, F&& doStuff) const
{
    static_assert(std::is_invocable_r_v<void, decltype(doStuff), NodeIndex, NodeIndex>, "Function needs to have signature void(NodeIndex from, NodeIndex to)");
    assert(toIndex < m_incomingAdjacencyList.size());
    for (auto fromIndex : m_incomingAdjacencyList[toIndex])
    {
        doStuff(fromIndex, toIndex);
    }
}

template<class NodeFunc, class EdgeFunc>
inline void Graph::ForEachNodeAndOutgoingEdge(NodeFunc&& nodeFunc, EdgeFunc&& edgeFunc) const
{
    static_assert(std::is_invocable_r_v<void, decltype(nodeFunc), const Node&>, "NodeFunc needs to have signature void(const Node&)");
    static_assert(std::is_invocable_r_v<void, decltype(edgeFunc), NodeIndex, NodeIndex>, "EdgeFunc needs to have signature void(NodeIndex from, NodeIndex to)");
    
    for (size_t i = 0; i < m_nodes.size(); ++i)
    {
        if (m_nodes[i].GetNodeType() == NodeType::kMaxTypes)
        {
            continue;
        }
        nodeFunc(m_nodes[i]);
        ForEachOutEdge(i, std::forward<EdgeFunc>(edgeFunc));
    }
}

template<class NodeFunc, class InEdgeFunc, class OutEdgeFunc>
inline void Graph::ForEachNodeAndEdge(NodeFunc&& nodeFunc, InEdgeFunc&& incomingEdgeFunc, OutEdgeFunc&& outgoingEdgeFunc) const
{
    static_assert(std::is_invocable_r_v<void, decltype(nodeFunc), const Node&>, "NodeFunc needs to have signature void(const Node&)");
    static_assert(std::is_invocable_r_v<void, decltype(incomingEdgeFunc), NodeIndex, NodeIndex>, "EdgeFunc needs to have signature void(NodeIndex from, NodeIndex to)");
    static_assert(std::is_invocable_r_v<void, decltype(outgoingEdgeFunc), NodeIndex, NodeIndex>, "EdgeFunc needs to have signature void(NodeIndex from, NodeIndex to)");

    for (size_t i = 0; i < m_nodes.size(); ++i)
    {
        if (m_nodes[i].GetNodeType() == NodeType::kMaxTypes)
        {
            continue;
        }

        nodeFunc(m_nodes[i]);
        ForEachInEdge(i, std::forward<InEdgeFunc>(incomingEdgeFunc));
        ForEachOutEdge(i, std::forward<OutEdgeFunc>(outgoingEdgeFunc));
    }
}

template<class EdgeFunc, class DiscoverFunc>
inline void Graph::RunBFS(NodeIndex startNode, EdgeFunc&& visitEdge, DiscoverFunc&& onDiscover) const
{
    static_assert(std::is_invocable_r_v<void, decltype(visitEdge), NodeIndex, NodeIndex>, "EdgeFunc needs to have signature void(NodeIndex, NodeIndex)");
    static_assert(std::is_invocable_r_v<void, decltype(onDiscover), NodeIndex>, "DiscoverFunc needs to have signature void(NodeIndex discovered)");
    std::unordered_set<NodeIndex> discoveredSet;
    std::queue<NodeIndex> indicesQueue;
    discoveredSet.emplace(startNode);
    indicesQueue.emplace(startNode);
    onDiscover(startNode);

    while (!indicesQueue.empty())
    {
        NodeIndex currentNode = indicesQueue.front();
        indicesQueue.pop();

        ForEachOutEdge(currentNode, [&discoveredSet, &indicesQueue, &onDiscover, &visitEdge](NodeIndex from, NodeIndex to)
            {
                if (discoveredSet.count(to) == 0)
                {
                    discoveredSet.emplace(to);
                    indicesQueue.emplace(to);
                    onDiscover(to);
                }

                visitEdge(from, to);
            });
    }
}
