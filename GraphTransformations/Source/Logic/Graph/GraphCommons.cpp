#include "GraphCommons.h"
#include <Utils/StringHash.h>

NodeType FromString(std::string_view str)
{
    using namespace yang::literals;

    switch (StringHash32(str.data()))
    {
    case "Entrance"_hash32: return NodeType::kEntrance;
    case "Boss"_hash32: return NodeType::kBoss;
    case "Item"_hash32: return NodeType::kItem;
    case "Goal"_hash32: return NodeType::kGoal;
    case "Start"_hash32: return NodeType::kStart;
    case "Task"_hash32: return NodeType::kTask;
    case "Monster"_hash32: return NodeType::kMonster;
    case "Puzzle"_hash32: return NodeType::kPuzzle;
    case "Rest"_hash32: return NodeType::kRest;
    case "Treasure"_hash32: return NodeType::kTreasure;
    case "Miniboss"_hash32: return NodeType::kMiniboss;
    default: return NodeType::kMaxTypes;
    }
}

NodeType FromString(const std::string& str)
{
    return FromString(std::string_view(str));
}

std::string ToString(NodeType type)
{
    switch (type)
    {
    case NodeType::kEntrance: return "Entrance";
    case NodeType::kBoss: return "Boss";
    case NodeType::kItem: return "Item";
    case NodeType::kGoal: return "Goal";
    case NodeType::kStart: return "Start";
    case NodeType::kTask: return "Task";
    case NodeType::kMiniboss: return "Miniboss";
    case NodeType::kMonster: return "Monster";
    case NodeType::kPuzzle: return "Puzzle";
    case NodeType::kRest: return "Rest";
    case NodeType::kTreasure: return "Treasure";
    default: return "Invalid";
    }
}
