#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <Utils/Color.h>

using NodeId = uint64_t;
using NodeIndex = uint32_t;
using NodeRefCount = uint32_t;

namespace yang
{
    class Sprite;
    class IFont;
}

enum class NodeType
{
    kEntrance,
    kBoss,
    kItem,
    kGoal,
    kStart,
    kTask,
    kMiniboss,
    kMonster,
    kPuzzle,
    kRest,
    kTreasure,
    kMaxTypes
};

NodeType FromString(std::string_view str);
NodeType FromString(const std::string& str);
std::string ToString(NodeType type);
yang::IColor ColorFromNodeType(NodeType type);
std::shared_ptr<yang::Sprite> SpriteFromNodeType(NodeType type, std::shared_ptr<yang::IFont> pFont);