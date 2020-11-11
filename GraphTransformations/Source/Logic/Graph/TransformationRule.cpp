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
    m_weight = pData->IntAttribute("weight", 0);
    m_shouldUseOnce = pData->BoolAttribute("shouldUseOnce", false);

    if (const char* pName = pData->Attribute("name"); pName != nullptr)
    {
        m_name = pName;
    }

    if (XMLElement* pFrom = pData->FirstChildElement("From"); pFrom != nullptr)
    {
        if (const char* pFromGraphSrc = pFrom->Attribute("src"); pFromGraphSrc != nullptr)
        {
            m_nodePattern = Graph::BuildGraphFromXML(std::string_view(pFromGraphSrc));
        }
        else
        {
            LOG(Error, "No \'From\' graph provided, failed to initialize rule from XML");
            m_isValid = false;
        }
    }
    else
    {
        LOG(Error, "No \'From\' graph, failed to initialize rule from XML");
        m_isValid = false;
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
            m_isValid = false;
        }
    }
    else
    {
        LOG(Error, "No \'To\' graph provided, failed to initialize rule from XML");
        m_isValid = false;
    }

    if (XMLElement* pReplacementMap = pData->FirstChildElement("ReplacementMap"); pReplacementMap != nullptr)
    {
        for (XMLElement* pReplacement = pReplacementMap->FirstChildElement("Replacement"); pReplacement != nullptr; pReplacement = pReplacement->NextSiblingElement("Replacement"))
        {
            int from = pReplacement->IntAttribute("from", -1);
            int to = pReplacement->IntAttribute("to", -1);

            if (from == -1 || to == -1)
            {
                LOG(Error, "Failed to read replacement from XML");
                m_isValid = false;
            }

            m_nodeReplacementMap[from] = to;
        }

        if (m_nodeReplacementMap.size() != m_nodePattern.Size())
        {
            LOG(Error, "Not all source nodes mapped to output");
            m_isValid = false;
        }
    }
    else
    {
        LOG(Error, "No replacement map provided, failed to initialize rule from XML");
        m_isValid = false;
    }
}
