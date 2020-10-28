#include "RegularGrammar.h"
#include "../Utils/TinyXml2/tinyxml2.h"

bool RegularGrammar::Init(std::string_view pathToXml)
{
    m_ruleContainer = RuleContainer();

    using namespace tinyxml2;

    XMLDocument doc;
    XMLError error = doc.LoadFile(pathToXml.data());

    if (error != tinyxml2::XML_SUCCESS)
    {
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

    return true;
}

Weapon RegularGrammar::RunGrammar()
{
    if (m_initialState == 0)
        return Weapon();

    std::string result(1, m_initialState);
    auto rngLambda = [this]() {return m_rngDevice.FRand<float>(); };

    while (!result.empty() && m_nonTerminatingSet.count(result.back()))
    {
        char c = result.back();
        result.pop_back();
        result.append(m_ruleContainer.GetRuleResult(c, rngLambda));
    }

    return Weapon(result, m_rngDevice);
}
