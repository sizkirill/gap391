#include "TransformationRule.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <cassert>
#include <Utils/StringHash.h>
#include <Utils/Logger.h>

TransformationRule::TransformationRule(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;
    using namespace yang::literals;

    assert(StringHash32(pData->Name()) == "Rule"_hash32);
    m_weight = pData->FloatAttribute("weight", 0.f);
    m_shouldUseOnce = pData->BoolAttribute("shouldUseOnce", false);

    if (XMLElement* pFrom = pData->FirstChildElement("From"); pFrom != nullptr)
    {
        if (const char* pFromNodeType = pFrom->Attribute("type"); pFromNodeType != nullptr)
        {
            m_sourceNodeType = FromString(std::string_view(pFromNodeType));
        }
        else
        {
            LOG(Error, "No \'From\' node type provided, failed to initialize rule from XML");
            m_sourceNodeType = NodeType::kMaxTypes;
        }
    }
    else
    {
        LOG(Error, "No \'From\' node provided, failed to initialize rule from XML");
        m_sourceNodeType = NodeType::kMaxTypes;
    }

    if (XMLElement* pTo = pData->FirstChildElement("To"); pTo != nullptr)
    {
        if (const char* pToGraphSrc = pTo->Attribute("src"); pToGraphSrc != nullptr)
        {
            m_resultGraph = Graph::BuildGraphFromXML(std::string_view(pToGraphSrc));
        }
        else
        {
            LOG(Error, "No \'To\' graph provided, failed to initialize rule from XML");
            m_sourceNodeType = NodeType::kMaxTypes;
        }
    }
    else
    {
        LOG(Error, "No \'To\' graph provided, failed to initialize rule from XML");
        m_sourceNodeType = NodeType::kMaxTypes;
    }
}
