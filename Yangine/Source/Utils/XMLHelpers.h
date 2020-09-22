#pragma once
#include <Utils/Vector2.h>

namespace tinyxml2
{
    class XMLElement;
}

namespace yang
{
    /// Helper function to read Vector2 from XML element
    /// \param pData - XML element to read from
    /// \return Vector2 from the xml element
    FVec2 FVectorFromXML(tinyxml2::XMLElement* pData);

    /// Helper function to read Vector2 from XML element
    /// \param pData - XML element to read from
    /// \return Vector2 from the xml element
    IVec2 IVectorFromXML(tinyxml2::XMLElement* pData);

    /// Helper function to read angle from XML element. Converts degrees to radians
    /// \param pData - XML element to read from
    /// \param attributeName - name of the attribute that contains angle
    /// \return angle in radians
    float AngleFromXML(tinyxml2::XMLElement* pData, const char* attributeName);
}