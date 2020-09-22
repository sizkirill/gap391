#include "XMLHelpers.h"
#include <Utils/TinyXml2/tinyxml2.h>

using yang::FVec2;
using yang::IVec2;

FVec2 yang::FVectorFromXML(tinyxml2::XMLElement* pData)
{
    FVec2 result(0,0);
    if (pData)
    {
        result.x = pData->FloatAttribute("x");
        result.y = pData->FloatAttribute("y");
    }

    return result;
}

IVec2 yang::IVectorFromXML(tinyxml2::XMLElement* pData)
{
    IVec2 result(0, 0);
    if (pData)
    {
        result.x = pData->IntAttribute("x");
        result.y = pData->IntAttribute("y");
    }

    return result;
}

float yang::AngleFromXML(tinyxml2::XMLElement* pData, const char* attributeName)
{
    return Math::ToRadians(pData->FloatAttribute(attributeName));
}
