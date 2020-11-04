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
        m_adjacencyLists.emplace_back();
    }

    return newId;
}

void Graph::DeleteNode(NodeId id)
{
    auto& node = GetNode(id);
    node.SetRefCount(0);
    m_freeList.push_back(GetIndex(id));
    m_adjacencyLists[GetIndex(id)].clear();
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

    for (size_t i = 0; i < m_adjacencyLists.size(); ++i)
    {
        if (m_adjacencyLists[i].count(index))
        {
            result.push_back(i);
            m_adjacencyLists[i].erase(index);
        }
    }

    m_nodes[index].SetRefCount(1);

    return result;
}

std::vector<NodeIndex> Graph::UnlinkOutgoingNodes(NodeIndex index)
{
    std::vector<NodeIndex> result;

    assert(index < m_adjacencyLists.size());
    for (auto nodeIndex : m_adjacencyLists[index])
    {
        result.push_back(nodeIndex);
        GetNode(nodeIndex).DecrementRef();
    }

    m_adjacencyLists[index].clear();
    
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

void Graph::LinkNodes(NodeIndex fromId, NodeIndex toId)
{
    auto& toNode = m_nodes[toId];

    m_adjacencyLists[fromId].emplace(toId);
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
