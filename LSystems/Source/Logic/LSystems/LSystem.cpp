#include "LSystem.h"
#include <Utils/Logger.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Logic/Scripting/LuaState.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/ApplicationConstants.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Input/KeyboardInputEvent.h>

LSystem::LSystem(yang::IGraphics* pGraphics)
{
    m_pTexture = pGraphics->CreateTexture({ yang::kWindowWidth, yang::kWindowHeight });
    m_eventListenerIndex = yang::EventDispatcher::Get()->AddEventListener(yang::KeyboardInputEvent::kEventId, [this, pGraphics](yang::IEvent* pEvent) {HandleInputEvent(pEvent, pGraphics); });
    m_initialSeed = m_rngDevice.GetState();
}

LSystem::~LSystem()
{
    if (m_eventListenerIndex != yang::kInvalidValue<size_t>)
    {
        yang::EventDispatcher::Get()->RemoveEventListener(yang::KeyboardInputEvent::kEventId, m_eventListenerIndex);
    }
}

bool LSystem::Init(std::string_view pathToXml)
{
    m_ruleContainer = RuleContainer();

    using namespace tinyxml2;

    XMLDocument doc;
    XMLError error = doc.LoadFile(pathToXml.data());

    if (error != tinyxml2::XML_SUCCESS)
    {
        LOG(Error, "Failed to load map settings: %s -- %s", pathToXml.data(), XMLDocument::ErrorIDToName(error));
        return false;
    }

    XMLElement* pRoot = doc.RootElement();

    for (XMLElement* pRule = pRoot->FirstChildElement("Rule"); pRule != nullptr; pRule = pRule->NextSiblingElement("Rule"))
    {
        const char* strVar = pRule->Attribute("var");

        if (!strVar)
        {
            continue;
        }

        const char* pRuleResult = pRule->Attribute("result");

        if (!pRuleResult)
        {
            continue;
        }

        float probability = pRule->FloatAttribute("probability", 1.f);

        m_ruleContainer.AddRule(strVar[0], probability, std::string(pRuleResult));
    }

    for (XMLElement* pConstant = pRoot->FirstChildElement("Constant"); pConstant != nullptr; pConstant = pConstant->NextSiblingElement("Constant"))
    {
        const char* strVar = pConstant->Attribute("var");

        if (!strVar)
        {
            continue;
        }

        m_ruleContainer.AddConstant(strVar[0]);
    }

    m_ruleContainer.Finalize();

    const char* pInitState = pRoot->Attribute("state");
    m_initialState = pInitState ? pInitState : "";
    m_state = m_initialState;
    
    if (XMLElement* pScriptSrcElement = pRoot->FirstChildElement("Ruleset"); pScriptSrcElement != nullptr)
    {
        const char* pScriptSrc = pScriptSrcElement->Attribute("src");

        if (pScriptSrc)
        {
            yang::LuaState::GetInstance()->DoFile(pScriptSrc);
            m_luaRuleFunc = yang::LuaState::GetInstance()->Get<yang::LuaCallback>();
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    XMLElement* pStartPos = pRoot->FirstChildElement("Position");
    XMLElement* pOrientation = pRoot->FirstChildElement("Orientation");

    if (pStartPos && pOrientation)
    {
        yang::FVec2 position = yang::FVectorFromXML(pStartPos);
        yang::FVec2 forward = yang::FVec2::FromAngle(yang::AngleFromXML(pOrientation, "angle"));
        m_lineBuilder.SetTransform(position, forward);
    }

    m_stepAmount = pRoot->FloatAttribute("stepAmount", 1.f);
    m_numIterations = pRoot->IntAttribute("numIterations", 4);
    m_initialSeed = pRoot->Unsigned64Attribute("seed", m_initialSeed);

    return true;
}

void LSystem::ProcessStep()
{
    auto rngLambda = [this]() {return m_rngDevice.FRand<float>(); };
    std::string newState;

    for (auto c : m_state)
    {
        newState.append(m_ruleContainer.GetRuleResult(c, rngLambda));
    }
    m_state = std::move(newState);

    ++m_currentIterations;
}

void LSystem::HandleInputEvent(yang::IEvent* pEvent, yang::IGraphics* pGraphics)
{
    if (pEvent->GetEventId() == yang::KeyboardInputEvent::kEventId)
    {
        auto pKeyboardEvent = static_cast<yang::KeyboardInputEvent*>(pEvent);
        if (pKeyboardEvent->GetEventType() == yang::KeyboardInputEvent::EventType::kKeyReleased)
        {
            if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kUP)
            {
                ++m_numIterations;
                Reset();
                Process();
                BuildLines(pGraphics);
            }

            else if (pKeyboardEvent->GetKeyCode() == yang::KeyboardInputEvent::KeyCode::kDOWN)
            {
                --m_numIterations;
                Reset();
                Process();
                BuildLines(pGraphics);
            }
        }
    }
}

void LSystem::BuildLines(yang::IGraphics* pGraphics)
{
    for (auto c : m_state)
    {
        m_luaRuleFunc.Call<void>(&m_lineBuilder, c, m_currentIterations, m_stepAmount, &m_rngDevice);
    }

    pGraphics->SetRenderTarget(m_pTexture.get());
    pGraphics->StartDrawing(0, 0, 0, 0xff);

    const auto& lineList = m_lineBuilder.GetLineList();

    for (size_t i = 0; i < lineList.size(); i += 2)
    {
        assert(i + 1 < lineList.size());

        pGraphics->DrawLine(lineList[i].first, lineList[i + 1].first, lineList[i].second);
    }

    pGraphics->SetRenderTarget(nullptr);
}

void LSystem::Render(yang::IGraphics* pGraphics)
{
    pGraphics->DrawTexture(m_pTexture.get());
}

void LSystem::Reset()
{
    m_currentIterations = 0;
    m_state = m_initialState;
    m_rngDevice = yang::XorshiftRNG(m_initialSeed);
    m_lineBuilder.Reset();
}

void LSystem::ResetRNG()
{
    m_rngDevice = yang::XorshiftRNG();
}

void LSystem::Process()
{
    while (m_currentIterations < m_numIterations)
    {
        ProcessStep();
    }
}
