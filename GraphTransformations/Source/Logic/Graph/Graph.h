#pragma once
#include <unordered_set>
#include <vector>
#include <type_traits>
#include <unordered_map>
#include <string_view>
#include <cassert>
#include "GraphCommons.h"

class TransformationRule;

struct Node
{
    NodeId m_nodeId;
    NodeType m_nodeType;

    NodeIndex GetIndex() const;
    NodeRefCount GetRefCount() const;
    NodeId SetRefCount(NodeRefCount refCount);
    void IncrementRef();
    void DecrementRef();
};

class Graph
{
public:
    NodeId AddNode(NodeType type);
    void DeleteNode(NodeId id);
    void SpliceSubGraph(NodeId nodeIdToReplace, const Graph& subGraph);

    std::vector<NodeIndex> FindNodeIndicesByType(NodeType type);

    static Graph BuildGraphFromXML(std::string_view pathToXml);

    template <class F>
    void ForEachNode(F&& doStuff) const;

    template <class F>
    void ForEachEdge(F&& doStuff) const;

    template <class F>
    void ForEachOutEdge(NodeIndex fromIndex, F&& doStuff) const;

    template <class NodeFunc, class EdgeFunc>
    void ForEachNodeAndEdge(NodeFunc&& nodeFunc, EdgeFunc&& edgeFunc) const;

private:
    std::vector<Node> m_nodes;
    std::vector<std::unordered_set<NodeIndex>> m_adjacencyLists;
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
    void LinkNodes(NodeIndex fromId, NodeIndex toId);
};

template<class F>
inline void Graph::ForEachNode(F&& doStuff) const
{
    static_assert(std::is_invocable_r_v<void, decltype(doStuff), const Node&>, "Function needs to have signature void(const Node&)");
    for (auto& node : m_nodes)
    {
        doStuff(node);
    }
}

template<class F>
inline void Graph::ForEachEdge(F&& doStuff) const
{
    static_assert(std::is_invocable_r_v<void, decltype(doStuff), NodeIndex, NodeIndex>, "Function needs to have signature void(NodeIndex from, NodeIndex to)");
    for(size_t fromIndex = 0; fromIndex < m_adjacencyLists.size(); ++fromIndex)
    {
        for (auto toIndex : m_adjacencyLists[fromIndex])
        {
            doStuff(fromIndex, toIndex);
        }
    }
}

template<class F>
inline void Graph::ForEachOutEdge(NodeIndex fromIndex, F&& doStuff) const
{
    static_assert(std::is_invocable_r_v<void, decltype(doStuff), NodeIndex, NodeIndex>, "Function needs to have signature void(NodeIndex from, NodeIndex to)");
    assert(fromIndex < m_adjacencyLists.size());
    for (auto toIndex : m_adjacencyLists[fromIndex])
    {
        doStuff(fromIndex, toIndex);
    }
}

template<class NodeFunc, class EdgeFunc>
inline void Graph::ForEachNodeAndEdge(NodeFunc&& nodeFunc, EdgeFunc&& edgeFunc) const
{
    static_assert(std::is_invocable_r_v<void, decltype(nodeFunc), const Node&>, "NodeFunc needs to have signature void(const Node&)");
    static_assert(std::is_invocable_r_v<void, decltype(edgeFunc), NodeIndex, NodeIndex>, "EdgeFunc needs to have signature void(NodeIndex from, NodeIndex to)");
    
    for (size_t i = 0; i < m_nodes.size(); ++i)
    {
        nodeFunc(m_nodes[i]);
        ForEachOutEdge(i, std::forward<EdgeFunc>(edgeFunc));
    }
}
