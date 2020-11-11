#pragma once
#include <type_traits>
#include "Graph.h"
#include "TransformationRule.h"

template <class Printer>
class GraphPrinter
{
public:
    GraphPrinter(Printer&& printer) : m_printer(std::forward<Printer>(printer)) {}

    void PrintGraph(const Graph& graph);
    void PrintRule(const TransformationRule& rule);
    void PrintIteration(const Graph& graph);
private:
    Printer m_printer;
    size_t m_currentIteration = 0;

    void PrintNode(const Node& node);
};

template<class Printer>
inline void GraphPrinter<Printer>::PrintNode(const Node& node)
{
    m_printer << "Index: [" << node.GetIndex() << "]. Type: " << ToString(node.m_nodeType) << "\n";
}

template<class Printer>
inline void GraphPrinter<Printer>::PrintGraph(const Graph& graph)
{
    graph.ForEachNodeAndEdge([this](const Node& node)
        {
            PrintNode(node);
        },
        [this](NodeIndex from, NodeIndex to)
        {
            m_printer << "        <- [" << from << "]\n";
        },
        [this](NodeIndex from, NodeIndex to)
        {
            m_printer << "        -> [" << to << "]\n";
        });
    m_printer << "\n";
}

template<class Printer>
inline void GraphPrinter<Printer>::PrintRule(const TransformationRule& rule)
{
    m_printer << "***Iteration #" << m_currentIteration << "***\n";
    m_printer << "Choosing rule: " << rule.GetName() << ".\n";
}

template<class Printer>
inline void GraphPrinter<Printer>::PrintIteration(const Graph& graph)
{
    PrintGraph(graph);
    ++m_currentIteration;
}
