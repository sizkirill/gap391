#include "RuleContainer.h"
#include <Utils/StringHash.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
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

    for (XMLElement* pRuleElement = pRoot->FirstChildElement("Rule"); pRuleElement = pRuleElement->NextSiblingElement("Rule"); pRuleElement != nullptr)
    {
        m_ruleContainer.emplace_back(pRuleElement);
    }
}
