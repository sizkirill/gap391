#include "RegularGrammar.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/ApplicationConstants.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Input/KeyboardInputEvent.h>

RegularGrammar::RegularGrammar(yang::IGraphics* pGraphics)
{
    m_pTexture = pGraphics->CreateTexture({ yang::kWindowWidth, yang::kWindowHeight });
    //m_eventListenerIndex = yang::EventDispatcher::Get()->AddEventListener(yang::KeyboardInputEvent::kEventId, [this, pGraphics](yang::IEvent* pEvent) {HandleInputEvent(pEvent, pGraphics); });
}

bool RegularGrammar::Init(std::string_view pathToXml)
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
        m_nonTerminatingSet.emplace(strVar[0]);
    }

    for (XMLElement* pConstant = pRoot->FirstChildElement("Constant"); pConstant != nullptr; pConstant = pConstant->NextSiblingElement("Constant"))
    {
        const char* strVar = pConstant->Attribute("var");

        if (!strVar)
        {
            continue;
        }

        m_ruleContainer.AddConstant(strVar[0]);
        m_terminatingSet.emplace(strVar[0]);
    }

    m_ruleContainer.Finalize();

    const char* pInitState = pRoot->Attribute("state");
    m_initialState = pInitState ? pInitState[0] : 0;
    m_state = std::string(1, m_initialState);

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
    m_maxStringSize = (size_t)(pRoot->IntAttribute("maxStringSize", 255));

    return true;
}

void RegularGrammar::RunGrammar()
{
    if (m_initialState == 0)
    {
        m_state = "";
        return;
    }

    auto rngLambda = [this]() {return m_rngDevice.FRand<float>(); };

    while (!m_state.empty() && m_nonTerminatingSet.count(m_state.back()) && m_state.size() < m_maxStringSize)
    {
        char c = m_state.back();
        m_state.pop_back();
        m_state.append(m_ruleContainer.GetRuleResult(c, rngLambda));
    }
}

void RegularGrammar::BuildLines(yang::IGraphics* pGraphics)
{
    for (auto c : m_state)
    {
        m_luaRuleFunc.Call<void>(&m_lineBuilder, c, m_stepAmount, &m_rngDevice);
    }

    pGraphics->SetRenderTarget(m_pTexture.get());
    pGraphics->StartDrawing(0, 0, 0, 0xff);

    const auto& lineList = m_lineBuilder.GetLineList();

    for (size_t i = 0; i < lineList.size(); i += 2)
    {
        assert(i + 1 < lineList.size());

        pGraphics->DrawLine(lineList[i].first, lineList[i + 1].first, lineList[i].second);
    }

    for (const auto& [rect, color] : m_lineBuilder.GetRectList())
    {
        pGraphics->DrawRect(rect, color);
    }

    pGraphics->SetRenderTarget(nullptr);
}

void RegularGrammar::Render(yang::IGraphics* pGraphics)
{
    pGraphics->DrawTexture(m_pTexture.get());
}

void RegularGrammar::Reset()
{
    m_state = m_initialState;
    m_rngDevice = yang::XorshiftRNG();
    m_lineBuilder.Reset();
}

void RegularGrammar::ResetRNG()
{
    m_rngDevice = yang::XorshiftRNG();
}
