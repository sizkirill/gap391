#pragma once
#include <Logic/LSystems/RuleContainer.h>
#include <string>
#include <memory>
#include <string_view>
#include <Utils/Random.h>
#include <Logic/LSystems/LineBuilder.h>
#include <Logic/Scripting/LuaCallback.h>

namespace yang
{
    class IGraphics;
    class ITexture;
    class IEvent;
}

class LSystem
{
public:
    LSystem(yang::IGraphics* pGraphics);
    ~LSystem();
    bool Init(std::string_view pathToXml);
    const std::string& GetState() const { return m_state; }
    void BuildLines(yang::IGraphics* pGraphics);
    void Render(yang::IGraphics* pGraphics);
    void Reset();
    void ResetRNG();
    void Process();
private:
    RuleContainer m_ruleContainer;
    LineBuilder m_lineBuilder;
    std::string m_initialState;

    yang::XorshiftRNG m_rngDevice;
    uint64_t m_initialSeed;

    yang::LuaCallback m_luaRuleFunc;
    std::shared_ptr<yang::ITexture> m_pTexture;
    std::string m_state;

    int m_currentIterations = 0;
    int m_numIterations;
    float m_stepAmount;
    size_t m_eventListenerIndex = yang::kInvalidValue<size_t>;

    void ProcessStep();
    void HandleInputEvent(yang::IEvent* pEvent, yang::IGraphics* pGraphics);
};
