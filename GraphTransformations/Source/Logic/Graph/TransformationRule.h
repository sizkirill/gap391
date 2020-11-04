#pragma once
#include "Graph.h"
#include "GraphCommons.h"
#include <string>
#include <string_view>

namespace tinyxml2
{
    class XMLElement;
}

class TransformationRule
{
public:
    TransformationRule(tinyxml2::XMLElement* pData);

    bool IsValid() const { return m_sourceNodeType != NodeType::kMaxTypes; }
private:
    NodeType m_sourceNodeType;
    Graph m_resultGraph;
    std::string m_name;
    int m_weight;
    bool m_shouldUseOnce;
public:
    const Graph& GetResultGraph() const { return m_resultGraph; }
    NodeType GetSourceNodeType() const { return m_sourceNodeType; }
    std::string_view GetName() const { return m_name; }
    int GetWeight() const { return m_weight; }
    bool ShouldBeUsedOnce() const { return m_shouldUseOnce; }
    void ResetWeight() { m_weight = 0; }
};