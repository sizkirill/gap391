#pragma once
#include <string>
#include <iostream>
#include <array>
#include <string_view>

class Weapon
{
public:
    enum class Type
    {
        kSword,
        kAxe,
        kBow,
        kMace,
        kDagger,
        kMaxTypes
    };

    enum class Element
    {
        kFlaming,
        kElectric,
        kFreezing,
        kAcidic,
        kNone,
        kMaxTypes
    };

    friend std::ostream& operator << (std::ostream& os, Weapon& w);

    Weapon() = default;

    template <class RandomDevice>
    Weapon(std::string_view generated, RandomDevice&& rd);
private:
    Type m_weaponType = Type::kMaxTypes;
    Element m_element = Element::kNone;
    bool m_hasProperty = false;
    int m_enchantmentValue = 0;

    static constexpr std::array<std::string_view, static_cast<size_t>(Type::kMaxTypes)> kTypeToProperty = { " of Sharpness", "Mighty ", "Slaying ", " of Protection", "Poison " };
    static constexpr std::array<std::string_view, static_cast<size_t>(Type::kMaxTypes)> kTypeToWeaponName = { "Sword", "Axe", "Bow", "Mace", "Dagger" };
    static constexpr std::array<std::string_view, static_cast<size_t>(Element::kMaxTypes)> kElementToString = { "Flaming ", "Electric ", "Freezing ", "Acidic ", ""};
    static Type TypeFromChar(char c);
};

template<class RandomDevice>
inline Weapon::Weapon(std::string_view generated, RandomDevice&& rd)
{
    static_assert(std::is_integral_v<decltype(std::declval<RandomDevice>()())>, "RandomDevice should produce integer values");

    for (auto c : generated)
    {
        switch (c)
        {
        case 'N':
        {
            m_enchantmentValue = rd() % 4;
            break;
        }
        case 'C':
        {
            m_enchantmentValue = 0 - (rd() % 3) - 1;
            break;
        }
        case 'E':
        {
            m_element = static_cast<Element>(rd() % static_cast<size_t>(Element::kNone));
            break;
        }
        case 'P':
        {
            m_hasProperty = true;
            break;
        }
        case 'S': [[fallthrough]];
        case 'A': [[fallthrough]];
        case 'B': [[fallthrough]];
        case 'M': [[fallthrough]];
        case 'D':
        {
            m_weaponType = TypeFromChar(c);
            break;
        }
        default:
        {
            std::cout << "Reached default in weapon construction. Invalid Weapon";
            break;
        }
        }
    }
}
