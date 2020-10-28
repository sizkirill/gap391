#pragma once
#include "RuleContainer.h"
#include "../Utils/Random.h"
#include "Weapon.h"
#include <unordered_set>
#include <string>
#include <string_view>

class RegularGrammar
{
public:
    bool Init(std::string_view pathToXml);
    Weapon RunGrammar();
private:
    RuleContainer m_ruleContainer;
    std::unordered_set<char> m_nonTerminatingSet;
    std::unordered_set<char> m_terminatingSet;
    char m_initialState;
    XorshiftRNG m_rngDevice;
};