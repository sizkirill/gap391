#pragma once
#include "Graph.h"
#include "GraphCommons.h"
#include <string>
#include <string_view>
#include <unordered_map>

namespace tinyxml2
{
    class XMLElement;
}

class TransformationRule
{
public:
    TransformationRule(tinyxml2::XMLElement* pData);

    bool IsValid() const { return m_isValid; }

private:
    Graph m_nodePattern; //effectively array of node for now
    Graph m_resultGraph;
    std::unordered_map<NodeIndex, NodeIndex> m_nodeReplacementMap;
    std::string m_name;
    int m_weight;
    bool m_shouldUseOnce;
    bool m_isValid;
public:
    const Graph& GetResultGraph() const { return m_resultGraph; }
    const Graph& GetSourceGraph() const { return m_nodePattern; }
    const auto& GetReplacementMap() const { return m_nodeReplacementMap; }
    std::string_view GetName() const { return m_name; }
    int GetWeight() const { return m_weight; }
    bool ShouldBeUsedOnce() const { return m_shouldUseOnce; }
    void ResetWeight() { m_weight = 0; }
};