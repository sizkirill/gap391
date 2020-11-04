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
    void PrintIteration(const TransformationRule& rule, const Graph& graph);
private:
    Printer m_printer;
    size_t m_currentIteration = 0;

    void PrintNode(const Node& node);
};

template<class Printer>
inline void GraphPrinter<Printer>::PrintNode(const Node& node)
{
    m_printer << "Index: [" << node.GetIndex() << "]. RefCount: [" << node.GetRefCount() << "]. Type: " << ToString(node.m_nodeType) << "\n";
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
            m_printer << "        -> [" << to << "]\n";
        });
    m_printer << "\n";
}

template<class Printer>
inline void GraphPrinter<Printer>::PrintIteration(const TransformationRule& rule, const Graph& graph)
{
    m_printer << "***Iteration #" << m_currentIteration << "***\n";
    m_printer << "Choosing rule: " << rule.GetName() << ".\n";
    PrintGraph(graph);
    ++m_currentIteration;
}
