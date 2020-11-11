#include "Graph.h"
#include <cassert>
#include <algorithm>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Utils/StringHash.h>
#include "TransformationRule.h"

NodeId Graph::AddNode(NodeType type)
{
    NodeId newId = GetFreeId();
    auto newIndex = GetIndex(newId);

    // new index must be either inside the node vector, or next to the last (thus equal to size())
    assert(newIndex <= m_nodes.size());

    if (newIndex < m_nodes.size())
    {
        m_nodes[newIndex] = { newId, type };
    }
    else
    {
        m_nodes.push_back({ newId, type });
        m_outgoingAdjacencyList.emplace_back();
        m_incomingAdjacencyList.emplace_back();
    }

    return newId;
}

void Graph::DeleteNode(NodeId id)
{
    auto& node = GetNode(id);
    node.SetRefCount(0);
    m_freeList.push_back(GetIndex(id));
    m_outgoingAdjacencyList[GetIndex(id)].clear();
    m_incomingAdjacencyList[GetIndex(id)].clear();
}

void Graph::SpliceSubGraph(NodeId nodeIdToReplace, const Graph& subGraph)
{
    const auto indexToReplace = GetIndex(nodeIdToReplace);
    
    const auto inList = UnlinkIncomingNodes(indexToReplace);
    const auto outList = UnlinkOutgoingNodes(indexToReplace);
    DeleteNode(nodeIdToReplace);

    const auto nodeMap = CreateIslandFromSubGraph(subGraph);

    for (auto inIndex : inList)
    {
        LinkNodes(inIndex, nodeMap.at(subGraph.m_entrance));
    }

    for (auto outIndex : outList)
    {
        LinkNodes(nodeMap.at(subGraph.m_exit), outIndex);
    }
}

void Graph::SpliceSubGraph(std::vector<NodeIndex>& pattern, const TransformationRule& rule)
{
    using EdgeList = std::unordered_set<NodeIndex>;
    using AdjacencyList = std::vector<std::unordered_set<NodeIndex>>;

    EdgeList entranceEdges = m_incomingAdjacencyList[pattern.front()];
    EdgeList exitEdges = m_outgoingAdjacencyList[pattern.back()];

    //EdgeList entranceEdges = UnlinkIncomingNodes(pattern.front());
    //EdgeList exitEdges = UnlinkOutgoingNodes(pattern.back());

    AdjacencyList incomingEdges;
    AdjacencyList outgoingEdges;

    for (size_t i = 0; i < pattern.size(); ++i)
    {
        auto nodeIndex = pattern[i];
        incomingEdges.emplace_back();
        outgoingEdges.emplace_back();

        if (i != 0) // 0 is the entrance node, constraint #3
        {
            for (auto incomingId : m_incomingAdjacencyList[nodeIndex])
            {
                if (std::find(pattern.begin(), pattern.end(), incomingId) == pattern.end())
                {
                    incomingEdges[i].emplace(incomingId);
                }
            }
        }

        if (i != pattern.size() - 1) // size-1 is the exit node, constraint #3
        {
            for (auto outgoingId : m_outgoingAdjacencyList[nodeIndex])
            {
                if (std::find(pattern.begin(), pattern.end(), outgoingId) == pattern.end())
                {
                    outgoingEdges[i].emplace(outgoingId);
                }
            }
        }

        DeleteNode(nodeIndex);
    }

    const auto& ruleResultGraph = rule.GetResultGraph();
    const auto& ruleReplacementMap = rule.GetReplacementMap();

    assert(pattern.size() == ruleReplacementMap.size() && pattern.size() == incomingEdges.size() && pattern.size() == outgoingEdges.size());

    auto replacementMap = CreateIslandFromSubGraph(ruleResultGraph);

    for (size_t i = 0; i < ruleReplacementMap.size(); ++i)
    {
        NodeIndex replacingSubGraphId = ruleResultGraph.m_nodes.at(ruleReplacementMap.at(i)).GetIndex();
        NodeIndex replacingLevelGraphId = replacementMap[replacingSubGraphId];

        if (ruleResultGraph.m_entrance != replacingSubGraphId)
        {
            for (auto neighborIndex : incomingEdges[i])
            {
                LinkNodes(neighborIndex, replacingLevelGraphId);
            }
        }

        if (ruleResultGraph.m_exit != replacingSubGraphId)
        {
            for (auto neighborIndex : outgoingEdges[i])
            {
                LinkNodes(replacingLevelGraphId, neighborIndex);
            }
        }
    }

    auto entranceId = replacementMap[ruleResultGraph.m_entrance];
    for (auto neighborId : entranceEdges)
    {
        LinkNodes(neighborId, entranceId);
    }

    auto exitId = replacementMap[ruleResultGraph.m_exit];
    for (auto neighborId : exitEdges)
    {
        LinkNodes(exitId, neighborId);
    }
}

std::vector<NodeIndex> Graph::FindNodeIndicesByType(NodeType type)
{
    std::vector<NodeIndex> result;
    for (size_t i = 0; i < m_nodes.size(); ++i)
    {
        if (m_nodes[i].m_nodeType == type)
        {
            result.push_back(i);
        }
    }

    return result;
}

bool Graph::Transform(const TransformationRule& rule, yang::XorshiftRNG& rng)
{
    auto validNodes = FindValidNodePatterns(rule.GetSourceGraph());

    if (validNodes.empty())
        return false;

    auto& chosenPattern = validNodes[rng.Rand<size_t>(0, validNodes.size())];

    SpliceSubGraph(chosenPattern, rule);

    return true;
}

size_t Graph::GetInDegree(NodeIndex index) const
{
    assert(index < m_incomingAdjacencyList.size());
    return m_incomingAdjacencyList[index].size();
}

size_t Graph::GetOutDegree(NodeIndex index) const
{
    assert(index < m_outgoingAdjacencyList.size());
    return m_outgoingAdjacencyList[index].size();
}

size_t Graph::GetDegree(NodeIndex index) const
{
    return GetInDegree(index) + GetOutDegree(index);
}

NodeType Graph::GetNodeType(NodeIndex index) const
{
    assert(index < m_nodes.size());
    return m_nodes[index].GetNodeType();
}

Node& Graph::GetNode(NodeId id)
{
    NodeIndex nodeIndex = GetIndex(id);
    assert(nodeIndex < m_nodes.size());
    return m_nodes[nodeIndex];
}

NodeIndex Graph::GetIndex(NodeId id) const
{
    return id & 0x0000'0000'ffff'ffff;
}

NodeRefCount Graph::GetRefCount(NodeId id) const
{
    return id >> 32;
}

NodeId Graph::SetRefCount(NodeId id, NodeRefCount refCount) const
{
    id &= 0x0000'0000'ffff'ffff;
    id |= ((NodeId)refCount << 32);
    return id;
}

NodeId Graph::GetFreeId()
{
    if (!m_freeList.empty())
    {
        NodeIndex newIndex = m_freeList.back();
        m_freeList.pop_back();
        return SetRefCount(newIndex, 1);
    }

    return static_cast<NodeId>(m_nodes.size()) | 0x0000'0001'0000'0000;
}

std::vector<NodeIndex> Graph::UnlinkIncomingNodes(NodeIndex index)
{
    std::vector<NodeIndex> result;

    assert(index < m_incomingAdjacencyList.size());
    for (auto nodeIndex : m_incomingAdjacencyList[index])
    {
        result.push_back(nodeIndex);
        m_outgoingAdjacencyList[nodeIndex].erase(index);
    }

    m_incomingAdjacencyList[index].clear();

    m_nodes[index].SetRefCount(0);

    return result;
}

std::vector<NodeIndex> Graph::UnlinkOutgoingNodes(NodeIndex index)
{
    std::vector<NodeIndex> result;

    assert(index < m_outgoingAdjacencyList.size());
    for (auto nodeIndex : m_outgoingAdjacencyList[index])
    {
        result.push_back(nodeIndex);
        m_incomingAdjacencyList[nodeIndex].erase(index);
        GetNode(nodeIndex).DecrementRef();
    }

    m_outgoingAdjacencyList[index].clear();
    
    return result;
}

std::unordered_map<NodeIndex, NodeIndex> Graph::CreateIslandFromSubGraph(const Graph& subGraph)
{
    std::unordered_map<NodeIndex, NodeIndex> result;
    
    subGraph.ForEachNode([&result, this](const Node& node)
        {
            auto newId = AddNode(node.m_nodeType);
            result.emplace(GetIndex(node.m_nodeId), GetIndex(newId));
        });

    subGraph.ForEachEdge([&result, this](NodeIndex from, NodeIndex to)
        {
            LinkNodes(result[from], result[to]);
        });

    return result;
}

Graph::NodeSequenceList Graph::FindValidNodePatterns(const Graph& pattern) const
{
    NodeSequenceList candidates;

    ForEachNode([&pattern, &candidates](const Node& node)
        {
            if (node.GetNodeType() == pattern.m_nodes[0].GetNodeType())
            {
                candidates.emplace_back(1, node.GetIndex());
            }
        });

    if (candidates.empty() || pattern.m_nodes.size() == 1)
    {
        return candidates;
    }

    NodeSequenceList validNodes;

    for (auto& candidate : candidates)
    {
        FindValidNodePatternsWithDepthLimitedSearch(candidate[0], pattern, validNodes);
    }

    return validNodes;
}

void Graph::FindValidNodePatternsWithDepthLimitedSearch(NodeIndex rootNodeId, const Graph& pattern, NodeSequenceList& outNodes) const
{
    DiscoveredSet discovered;
    NodeSequence workingSequence(pattern.Size(), yang::kInvalidValue<NodeIndex>);

    FindValidNodePatternsWithDepthLimitedSearchHelper(rootNodeId, discovered, pattern, 0, workingSequence, 0, outNodes);
}

void Graph::FindValidNodePatternsWithDepthLimitedSearchHelper(NodeIndex nodeId, DiscoveredSet& discovered, const Graph& pattern, size_t nodePatternIndex, NodeSequence& workingNodeSequence, size_t workingNodeSequenceIndex, NodeSequenceList& outNodes) const
{
    discovered.emplace(nodeId);

    if (m_nodes[nodeId].GetNodeType() == pattern.m_nodes[nodePatternIndex].GetNodeType())
    {
        workingNodeSequence[workingNodeSequenceIndex] = nodeId;

        // done?
        if (workingNodeSequenceIndex == pattern.m_nodes.size() - 1)
        {
            outNodes.push_back(workingNodeSequence);
            workingNodeSequence[workingNodeSequenceIndex] = yang::kInvalidValue<NodeIndex>;
            return;
        }

        for (auto neighborIndex : m_outgoingAdjacencyList[GetIndex(nodeId)])
        {
            if (discovered.count(neighborIndex) == 0)
            {
                FindValidNodePatternsWithDepthLimitedSearchHelper(neighborIndex, discovered, pattern, nodePatternIndex + 1, workingNodeSequence, workingNodeSequenceIndex + 1, outNodes);
            }
        }
    }
    else
    {
        workingNodeSequence[workingNodeSequenceIndex] = yang::kInvalidValue<NodeIndex>;
    }
}

void Graph::LinkNodes(NodeIndex fromId, NodeIndex toId)
{
    auto& toNode = m_nodes[toId];

    m_outgoingAdjacencyList[fromId].emplace(toId);
    m_incomingAdjacencyList[toId].emplace(fromId);
    toNode.IncrementRef();
}

Graph Graph::BuildGraphFromXML(std::string_view pathToXml)
{
    using namespace tinyxml2;
    using namespace yang::literals;

    XMLDocument doc;
    XMLError error = doc.LoadFile(pathToXml.data());

    if (error != tinyxml2::XML_SUCCESS)
    {
        LOG(Error, "Failed to load graph from XML: %s -- %s", pathToXml.data(), XMLDocument::ErrorIDToName(error));
        return Graph();
    }

    XMLElement* pRoot = doc.RootElement();
    assert(pRoot && (StringHash32(pRoot->Name()) == "Graph"_hash32));

    std::unordered_map<NodeIndex, NodeIndex> xmlIdToGraphIdMap;
    Graph result;

    for (auto pNodeElement = pRoot->FirstChildElement("Node"); pNodeElement != nullptr; pNodeElement = pNodeElement->NextSiblingElement("Node"))
    {
        const char* pNodeTypeStr = pNodeElement->Attribute("type");
        int xmlNodeId = pNodeElement->IntAttribute("id", -1);

        if (!pNodeTypeStr || xmlNodeId == -1)
        {
            LOG(Error, "Node id or node type is not provided, failed to build graph from XML file.");
            return Graph();
        }

        auto newNodeId = result.AddNode(FromString(std::string_view(pNodeTypeStr)));
        xmlIdToGraphIdMap.emplace(xmlNodeId, result.GetIndex(newNodeId));
    }

    for (auto pEdgeElement = pRoot->FirstChildElement("Edge"); pEdgeElement != nullptr; pEdgeElement = pEdgeElement->NextSiblingElement("Edge"))
    {
        int from = pEdgeElement->IntAttribute("from", -1);
        int to = pEdgeElement->IntAttribute("to", -1);

        if (from == -1 || to == -1)
        {
            LOG(Error, "Failed to initialize Edge from xml, fromId or toId were invalid");
            return Graph();
        }

        result.LinkNodes(xmlIdToGraphIdMap[from], xmlIdToGraphIdMap[to]);
    }

    if (XMLElement* pEntranceNode = pRoot->FirstChildElement("EntranceNode"); pEntranceNode != nullptr)
    {
        int entranceNodeId = pEntranceNode->IntAttribute("id", -1);
        if (entranceNodeId == -1)
        {
            LOG(Error, "Entrance node was -1, failed to initialize graph from XML");
            return result;
        }

        result.m_entrance = xmlIdToGraphIdMap[entranceNodeId];
    }

    if (XMLElement* pExitNode = pRoot->FirstChildElement("ExitNode"); pExitNode != nullptr)
    {
        int exitNodeId = pExitNode->IntAttribute("id", -1);
        if (exitNodeId == -1)
        {
            LOG(Error, "Exit node was -1, failed to initialize graph from XML");
            return result;
        }

        result.m_exit = xmlIdToGraphIdMap[exitNodeId];
    }

    return result;
}

NodeIndex Node::GetIndex() const
{
    return m_nodeId & 0x0000'0000'ffff'ffff;
}

NodeRefCount Node::GetRefCount() const
{
    return m_nodeId >> 32;
}

NodeId Node::SetRefCount(NodeRefCount refCount)
{
    NodeId newId = m_nodeId & 0x0000'0000'ffff'ffff;
    newId |= ((NodeId)refCount << 32);
    m_nodeId = newId;
    return newId;
}

void Node::IncrementRef()
{
    SetRefCount(GetRefCount() + 1);
}

void Node::DecrementRef()
{
    SetRefCount(GetRefCount() - 1);
}
