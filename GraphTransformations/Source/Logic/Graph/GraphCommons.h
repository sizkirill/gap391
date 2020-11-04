#pragma once
#include <cstdint>
#include <string>
#include <string_view>

using NodeId = uint64_t;
using NodeIndex = uint32_t;
using NodeRefCount = uint32_t;

enum class NodeType
{
    kEntrance,
    kBoss,
    kItem,
    kGoal,
    kStart,
    kTask,
    kMonster,
    kPuzzle,
    kRest,
    kTreasure,
    kMaxTypes
};

NodeType FromString(std::string_view str);
NodeType FromString(const std::string& str);
std::string ToString(NodeType type);