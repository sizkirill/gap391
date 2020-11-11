#include "GraphCommons.h"
#include <Utils/StringHash.h>
#include <Application/Graphics/Textures/Sprite.h>
#include <Application/Graphics/Fonts/IFont.h>

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

yang::IColor ColorFromNodeType(NodeType type)
{
    switch (type)
    {
    case NodeType::kEntrance: return 0xffff0000;
    case NodeType::kBoss: return 0xff000000;
    case NodeType::kItem: return 0xD2691E00;
    case NodeType::kGoal: return 0x33ffff00;
    case NodeType::kStart: return 0x66660000;
    case NodeType::kTask: return 0x0000ff00;
    case NodeType::kMiniboss: return 0xaa000000;
    case NodeType::kMonster: return 0x55000000;
    case NodeType::kPuzzle: return 0xdddddd00;
    case NodeType::kRest: return 0x00ff0000;
    case NodeType::kTreasure: return 0xD4AF3700;
    default: return yang::IColor::kBlack;
    }
}

std::shared_ptr<yang::Sprite> SpriteFromNodeType(NodeType type, std::shared_ptr<yang::IFont> pFont)
{
    switch (type)
    {
    case NodeType::kEntrance: return pFont->SpriteFromChar('E');
    case NodeType::kBoss: return pFont->SpriteFromChar('B');
    case NodeType::kItem: return pFont->SpriteFromChar('I');
    case NodeType::kGoal: return pFont->SpriteFromChar('G');
    case NodeType::kStart: return pFont->SpriteFromChar('S');
    case NodeType::kTask: return pFont->SpriteFromChar('T');
    case NodeType::kMiniboss: return pFont->SpriteFromChar('q');
    case NodeType::kMonster: return pFont->SpriteFromChar('m');
    case NodeType::kPuzzle: return pFont->SpriteFromChar('P');
    case NodeType::kRest: return pFont->SpriteFromChar('R');
    case NodeType::kTreasure: return pFont->SpriteFromChar('t');
    default: return pFont->SpriteFromChar('0');
    }
}

