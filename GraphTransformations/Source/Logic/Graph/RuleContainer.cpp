#include "RuleContainer.h"
#include <Utils/StringHash.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <numeric>
#include <cassert>

RuleContainer::RuleContainer(std::string_view pathToXml)
{
    using namespace tinyxml2;
    using namespace yang::literals;

    XMLDocument doc;
    XMLError error = doc.LoadFile(pathToXml.data());

    if (error != tinyxml2::XML_SUCCESS)
    {
        LOG(Error, "Failed to load RuleContainer from XML: %s -- %s", pathToXml.data(), XMLDocument::ErrorIDToName(error));
        return;
    }

    XMLElement* pRoot = doc.RootElement();
    assert(pRoot && (StringHash32(pRoot->Name()) == "Ruleset"_hash32));

    for (XMLElement* pRuleElement = pRoot->FirstChildElement("Rule"); pRuleElement != nullptr; pRuleElement = pRuleElement->NextSiblingElement("Rule"))
    {
        m_ruleContainer.emplace_back(pRuleElement);
    }
}

int RuleContainer::GetTotalWeight() const
{
    return std::accumulate(m_ruleContainer.begin(), m_ruleContainer.end(), 0, [](int weight, auto& rule) {return weight + rule.GetWeight(); });
}
