#include "Weapon.h"

std::ostream& operator<<(std::ostream& os, Weapon& w)
{
    os << Weapon::kElementToString[static_cast<size_t>(w.m_element)];

    switch (w.m_weaponType)
    {
    case Weapon::Type::kAxe: [[fallthrough]];
    case Weapon::Type::kBow: [[fallthrough]];
    case Weapon::Type::kDagger:
    {
        os << (w.m_hasProperty ? Weapon::kTypeToProperty[static_cast<size_t>(w.m_weaponType)] : "") << Weapon::kTypeToWeaponName[static_cast<size_t>(w.m_weaponType)];
        break;
    }
    case Weapon::Type::kSword: [[fallthrough]];
    case Weapon::Type::kMace:
    {
        os << Weapon::kTypeToWeaponName[static_cast<size_t>(w.m_weaponType)] << (w.m_hasProperty ? Weapon::kTypeToProperty[static_cast<size_t>(w.m_weaponType)] : "");
        break;
    }
    default:
    {
        os << "Failed\n";
        return os;
    }
    }

    if (w.m_enchantmentValue != 0)
    {
        os << ' ' << (w.m_enchantmentValue > 0 ? "+" : "") << w.m_enchantmentValue;
    }

    os << '\n';

    return os;
}

Weapon::Type Weapon::TypeFromChar(char c)
{
    switch (c)
    {
    case 'S': return Type::kSword;
    case 'M': return Type::kMace;
    case 'A': return Type::kAxe;
    case 'B': return Type::kBow;
    case 'D': return Type::kDagger;
    default:  return Type::kMaxTypes;
    }
}
