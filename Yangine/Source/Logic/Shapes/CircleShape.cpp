#include "CircleShape.h"
#include "RectangleShape.h"
#include "ConeShape.h"
#include <Application/Graphics/IGraphics.h>
#include <cassert>
#include <memory>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Logic/Components/TransformComponent.h>

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<yang::IShape> yang::IShape::CreateShape<yang::CircleShape::GetHashName()>(tinyxml2::XMLElement* pData) { return std::make_unique<CircleShape>(pData); };

#pragma warning(pop)

yang::CircleShape::CircleShape(FVec2 center, float radius, IColor color)
    :IShape(color)
    ,m_localCenter(center)
    ,m_radius(radius)
    ,m_center(FVec2(0,0))
{
}

yang::CircleShape::CircleShape(tinyxml2::XMLElement* pData)
    :m_center(FVec2(0,0))
{
    Init(pData);
}

bool yang::CircleShape::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;
    m_radius = pData->FloatAttribute("radius");
    m_localCenter = yang::FVectorFromXML(pData->FirstChildElement("Center"));

    XMLElement* pColor = pData->FirstChildElement("Color");

    if (pColor)
    {
        m_color = yang::IColor(pColor->IntAttribute("r"), pColor->IntAttribute("g"), pColor->IntAttribute("b"), pColor->IntAttribute("a", 255));
    }
    return true;
}

bool yang::CircleShape::Render(IGraphics* pGraphics)
{
    return pGraphics->FillCircle(m_center + m_localCenter, m_radius, m_color);
}

bool yang::CircleShape::DebugDraw(IGraphics* pGraphics)
{
    return pGraphics->DrawCircle(m_center + m_localCenter, m_radius, m_color);
}

bool yang::CircleShape::Collide(IShape* pOther)
{
    assert(pOther);
    return pOther->Collide(this);
}

bool yang::CircleShape::Collide(CircleShape* pOther)
{
    assert(pOther);
    return (pOther->GetCenter() - GetCenter()).SqrdLength() < (m_radius + pOther->m_radius) * (m_radius + pOther->m_radius);
}

bool yang::CircleShape::Collide(RectangleShape* pOther)
{
    assert(pOther);
    
    auto sqrd = [](float val) {return val * val; };

    auto rectVertices = pOther->GetVertices();

    for (const auto& vertex : rectVertices)
    {
        if (Contains(vertex))
            return true;
    }

    FVec2 actualCenter = GetCenter();

    bool xOverlap = !(actualCenter.x < rectVertices[0].x || actualCenter.x > rectVertices[1].x);
    bool yOverlap = !(actualCenter.y < rectVertices[0].y || actualCenter.y > rectVertices[1].y);

    if (yOverlap && (std::fabs(actualCenter.x - pOther->GetCenter().x) < std::fabs(m_radius + pOther->GetDimensions().x / 2)))
        return true;

    if (xOverlap && (std::fabs(actualCenter.y - pOther->GetCenter().y) < std::fabs(m_radius + pOther->GetDimensions().y / 2)))
        return true;

    return false;
}

bool yang::CircleShape::Collide(ConeShape* pOther)
{
    assert(pOther);
    return pOther->Collide(this);
}

bool yang::CircleShape::Contains(FVec2 point)
{
    return (GetCenter() - point).SqrdLength() < m_radius * m_radius;
}

void yang::CircleShape::Update(yang::TransformComponent* pTransform)
{
    m_center = pTransform->GetPosition();
}
