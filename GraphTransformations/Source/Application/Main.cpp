#include <Logic/Graph/GraphPrinter.h>
#include <Logic/Graph/Graph.h>
#include <Logic/Graph/RuleContainer.h>
#include <Utils/Random.h>
#include <conio.h>
#include <iostream>

#ifdef _DEBUG
#include <VLD/vld.h>
#endif

int main(int argc, const char** argv)
{
    GraphPrinter<std::ostream&> printer(std::cout);
    RuleContainer rules("Assets/Graphs/Ruleset.xml");
    Graph initialGraph = Graph::BuildGraphFromXML("Assets/Graphs/Graph.xml");

    yang::XorshiftRNG rng;

    char exit = 0;
    do 
    {
        // Get random rule based on weights
        // The initial start->startingGraph rule has a huge weight, so it is most likely the first one to run, but random may think otherwise =/ as a hack I'll leave it like that
        TransformationRule& rule = rules.GetWeightedRandomRule(rng);

        // get possible indices of a graph where we can apply the rule
        auto possibleIndices = initialGraph.FindNodeIndicesByType(rule.GetSourceNodeType());

        // if there are none, continue to next iteration (there is always possible that entrance will expand into entrance->task, so the loop won't be infinite)
        if (possibleIndices.size() > 0)
        {
            auto index = possibleIndices[rng.Rand(possibleIndices.size())];
            initialGraph.SpliceSubGraph(index, rule.GetResultGraph());
            printer.PrintIteration(rule, initialGraph);
            exit = _getch();
        }
        else
        {
            continue;
        }
    } while (exit != 'q');

    return 0;
}