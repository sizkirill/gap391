#include "ConeShape.h"
#include "RectangleShape.h"
#include <Application/Graphics/IGraphics.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Logic/Components/TransformComponent.h>

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<yang::IShape> yang::IShape::CreateShape<yang::ConeShape::GetHashName()>(tinyxml2::XMLElement* pData) { return std::make_unique<ConeShape>(pData); };

#pragma warning(pop)

yang::ConeShape::ConeShape(FVec2 center, float radius, float fov, float orientation, IColor color)
    :CircleShape(center, radius, color)
    ,m_fieldOfViewAngle(fov)
    ,m_orientationAngle(orientation)
{
}

yang::ConeShape::ConeShape(tinyxml2::XMLElement* pData)
    :CircleShape(pData)
{
    Init(pData);
}

bool yang::ConeShape::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    m_radius = pData->FloatAttribute("radius");
    m_orientationAngle = yang::AngleFromXML(pData, "orientation");
    m_fieldOfViewAngle = yang::AngleFromXML(pData, "fov");

    m_localCenter = yang::FVectorFromXML(pData->FirstChildElement("Center"));

    XMLElement* pColor = pData->FirstChildElement("Color");

    if (pColor)
    {
        m_color = yang::IColor(pColor->IntAttribute("r"), pColor->IntAttribute("g"), pColor->IntAttribute("b"), pColor->IntAttribute("a", 255));
    }
    return true;
}

bool yang::ConeShape::Render(IGraphics* pGraphics)
{
    return true;
}

bool yang::ConeShape::DebugDraw(IGraphics* pGraphics)
{
    FVec2 actualCenter = GetCenter();
    FVec2 firstPoint = actualCenter + m_radius * FVec2(std::cos(m_orientationAngle - m_fieldOfViewAngle / 2), std::sin(m_orientationAngle - m_fieldOfViewAngle / 2));
    FVec2 centerPoint = actualCenter + m_radius * FVec2(std::cos(m_orientationAngle), std::sin(m_orientationAngle));
    FVec2 secondPoint = actualCenter + m_radius * FVec2(std::cos(m_orientationAngle + m_fieldOfViewAngle / 2), std::sin(m_orientationAngle + m_fieldOfViewAngle / 2));
    
    return pGraphics->DrawPolygon({ actualCenter, firstPoint, centerPoint, secondPoint }, m_color);
}

bool yang::ConeShape::Collide(IShape* pOther)
{
    return pOther->Collide(this);
}

bool yang::ConeShape::Collide(CircleShape* pOther)
{
    FVec2 actualCenter = GetCenter();
    // If circles don't collide - false
    if (!CircleShape::Collide(pOther))
    {
        return false;
    }

    // if cone center is inside the circle the 
    if (pOther->Contains(actualCenter))
    {
        return true;
    }

    FVec2 directionToOther = pOther->GetCenter() - actualCenter;

    float angleToOther = std::fmod(std::atan2(directionToOther.y, directionToOther.x) + 2 * Math::kPi, 2 * Math::kPi); // [0, 2pi]

    angleToOther -= m_orientationAngle;

    if (angleToOther > -m_fieldOfViewAngle / 2 && angleToOther < m_fieldOfViewAngle / 2)
    {
        return true;
    }

    // some straight forward and probably not very efficient math for the edge case, just for the purpose for it to work
    FVec2 firstPoint = actualCenter + m_radius * FVec2(std::cos(m_orientationAngle - m_fieldOfViewAngle / 2), std::sin(m_orientationAngle - m_fieldOfViewAngle / 2));
    FVec2 secondPoint = actualCenter + m_radius * FVec2(std::cos(m_orientationAngle + m_fieldOfViewAngle / 2), std::sin(m_orientationAngle + m_fieldOfViewAngle / 2));

    if (pOther->Contains(firstPoint) || pOther->Contains(secondPoint))
    {
        return true;
    }

    return false;
}

bool yang::ConeShape::Collide(RectangleShape* pOther)
{
    if (!CircleShape::Collide(pOther))
    {
        return false;
    }

    FVec2 actualCenter = GetCenter();

    for (auto& vertex : pOther->GetVertices())
    {
        if (Contains(vertex))
        {
            return true;
        }
    }

    if (pOther->Contains(actualCenter))
    {
        return true;
    }

    // Okay, that's not quite right, but gives an okay approximation. Still it's not very efficient.
    FVec2 firstPoint = actualCenter + m_radius * FVec2(std::cos(m_orientationAngle - m_fieldOfViewAngle / 2), std::sin(m_orientationAngle - m_fieldOfViewAngle / 2));
    if (pOther->Contains(firstPoint))
    {
        return true;
    }

    FVec2 centerPoint = actualCenter + m_radius * FVec2(std::cos(m_orientationAngle), std::sin(m_orientationAngle));
    if (pOther->Contains(centerPoint))
    {
        return true;
    }

    FVec2 secondPoint = actualCenter + m_radius * FVec2(std::cos(m_orientationAngle + m_fieldOfViewAngle / 2), std::sin(m_orientationAngle + m_fieldOfViewAngle / 2));
    if (pOther->Contains(secondPoint))
    {
        return true;
    }

    return false;
}

bool yang::ConeShape::Collide(ConeShape* pOther)
{
    // Whatever. Let's suppose cones don't collide haha
    return false;
}

bool yang::ConeShape::Contains(FVec2 point)
{
    if (!CircleShape::Contains(point))
    {
        return false;
    }

    FVec2 toPoint = point - GetCenter();
    float angleToPoint = std::fmod(std::atan2(toPoint.y, toPoint.x) + 2 * Math::kPi, 2 * Math::kPi);    // angle from (0,1) to toPoint in range [0,2*pi]

    float angleDiff = std::fmod(std::fabs(angleToPoint - m_orientationAngle), 2 * Math::kPi);

    if (angleDiff > Math::kPi)
    {
        angleDiff = 2 * Math::kPi - angleDiff;
    }

    return angleDiff < m_fieldOfViewAngle / 2.f;
}

void yang::ConeShape::Update(TransformComponent* pTransform)
{
    CircleShape::Update(pTransform);

    m_orientationAngle = pTransform->GetRotation();
}

void yang::ConeShape::SetOrientation(float orientation)
{
    m_orientationAngle = std::fmod(orientation + 2 * Math::kPi, 2 * Math::kPi);
}

void yang::ConeShape::SetAngle(float angle)
{
    m_fieldOfViewAngle = std::fmod(angle, 2 * Math::kPi);
}
