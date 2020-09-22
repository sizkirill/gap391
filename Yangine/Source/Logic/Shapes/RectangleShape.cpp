#include "RectangleShape.h"
#include <Application/Graphics/IGraphics.h>
#include <Logic/Shapes/CircleShape.h>
#include <Logic/Shapes/ConeShape.h>
#include <Logic/Shapes/RectangleShape.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Logic/Components/TransformComponent.h>

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<yang::IShape> yang::IShape::CreateShape<yang::RectangleShape::GetHashName()>(tinyxml2::XMLElement* pData) { return std::make_unique<RectangleShape>(pData); };

#pragma warning(pop)

yang::RectangleShape::RectangleShape(FVec2 center, FVec2 dimensions, IColor color)
    :IShape(color)
    ,m_localCenter(center)
    ,m_dimensions(dimensions)
    ,m_center(FVec2(0,0))
{
}

yang::RectangleShape::RectangleShape(tinyxml2::XMLElement* pData)
    :m_center(FVec2(0,0))
{
    Init(pData);
}

bool yang::RectangleShape::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;
    m_localCenter = FVectorFromXML(pData->FirstChildElement("Center"));
    m_dimensions = FVectorFromXML(pData->FirstChildElement("Dimensions"));
    
    XMLElement* pColor = pData->FirstChildElement("Color");

    if (pColor)
    {
        m_color = yang::IColor(pColor->IntAttribute("r"), pColor->IntAttribute("g"), pColor->IntAttribute("b"), pColor->IntAttribute("a", 255));
    }

    return true;
}

bool yang::RectangleShape::Render(IGraphics* pGraphics)
{
    return pGraphics->FillRect(GetRect(), m_color);
}

bool yang::RectangleShape::DebugDraw(IGraphics* pGraphics)
{
    return pGraphics->DrawRect(GetRect(), m_color);
}

bool yang::RectangleShape::Collide(IShape* pOther)
{
    return pOther->Collide(this);
}

bool yang::RectangleShape::Collide(CircleShape* pOther)
{
    return pOther->Collide(this);
}

bool yang::RectangleShape::Collide(RectangleShape* pOther)
{
    return GetRect().Collide(pOther->GetRect());
}

bool yang::RectangleShape::Collide(ConeShape* pOther)
{
    return pOther->Collide(this);
}

bool yang::RectangleShape::Contains(FVec2 point)
{
    return GetRect().Contains(point);
}

void yang::RectangleShape::Update(TransformComponent* pTransform)
{
    m_center = pTransform->GetPosition();
}

std::array<yang::FVec2, 4> yang::RectangleShape::GetVertices() const
{
    FVec2 actualCenter = GetCenter();
    FVec2 topLeft = actualCenter - m_dimensions / 2.f;
    FVec2 topRight = actualCenter + FVec2(m_dimensions.x, -m_dimensions.y) / 2.f;
    FVec2 downLeft = actualCenter + FVec2(-m_dimensions.x, m_dimensions.y) / 2.f;
    FVec2 downRight = actualCenter + m_dimensions / 2.f;

    return { topLeft, topRight, downRight, downLeft };
}

yang::FRect yang::RectangleShape::GetRect() const
{
    FVec2 actualCenter = GetCenter();
    FRect toReturn;
    toReturn.x = actualCenter.x - m_dimensions.x / 2;
    toReturn.y = actualCenter.y - m_dimensions.y / 2;
    toReturn.height = m_dimensions.y;
    toReturn.width = m_dimensions.x;
    return toReturn;
}
