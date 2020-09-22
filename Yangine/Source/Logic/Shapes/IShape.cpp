#include "IShape.h"
#include "CircleShape.h"
#include "ConeShape.h"
#include "RectangleShape.h"
#include <Utils/StringHash.h>

std::unique_ptr<yang::IShape> yang::IShape::CreateShape(const char* shapeName, tinyxml2::XMLElement* pData)
{
    switch (StringHash32(shapeName))
    {
    case CircleShape::GetHashName():
        return CreateShape<CircleShape::GetHashName()>(pData);
    case ConeShape::GetHashName():
        return CreateShape<ConeShape::GetHashName()>(pData);
    case RectangleShape::GetHashName():
        return CreateShape<RectangleShape::GetHashName()>(pData);
    default:
        return nullptr;
    }
}

yang::IShape::IShape(IColor color)
    :m_color(color)
{
}
