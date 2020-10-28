#pragma once
#include "RuleContainer.h"
#include <Utils/Random.h>
#include <Logic/Scripting/LuaCallback.h>
#include <unordered_set>
#include <string>
#include <string_view>
#include <Utils/Typedefs.h>
#include <Logic/Grammar/LineBuilder.h>

namespace yang
{
    class ITexture;
    class IGraphics;
}

class RegularGrammar
{
public:
    RegularGrammar(yang::IGraphics* pGraphics);
    bool Init(std::string_view pathToXml);
    void RunGrammar();
    void BuildLines(yang::IGraphics* pGraphics);
    void Render(yang::IGraphics* pGraphics);
    void Reset();
    void ResetRNG();
    
    LineBuilder& GetLineBuilder() { return m_lineBuilder; }
private:
    RuleContainer m_ruleContainer;
    LineBuilder m_lineBuilder;
    std::unordered_set<char> m_nonTerminatingSet;
    std::unordered_set<char> m_terminatingSet;
    char m_initialState;
    std::string m_state;

    size_t m_maxStringSize;

    yang::XorshiftRNG m_rngDevice;

    yang::LuaCallback m_luaRuleFunc;
    std::shared_ptr<yang::ITexture> m_pTexture;

    float m_stepAmount;
    //size_t m_eventListenerIndex = yang::kInvalidValue<size_t>;
};