#pragma once
#include <Utils/Color.h>
#include <Utils/Vector2.h>
#include <memory>

namespace tinyxml2
{
    class XMLElement;
}

namespace yang
{

class IGraphics;
class CircleShape;
class RectangleShape;
class ConeShape;
class TransformComponent;

class IShape
{
public:
    template<uint32_t ShapeHashName, class... Args>
    static std::unique_ptr<IShape> CreateShape(Args... args);

    static std::unique_ptr<IShape> CreateShape(const char* shapeName, tinyxml2::XMLElement* pData);
public:
    IShape(IColor color = {0,0,0,0});
    virtual ~IShape() = default;
    virtual bool Init(tinyxml2::XMLElement* pData) = 0;
    virtual bool Render(IGraphics* pGraphics) = 0;
    virtual bool DebugDraw(IGraphics* pGraphics) = 0;

    virtual bool Collide(IShape* pOther) = 0;
    virtual bool Collide(CircleShape* pOther) = 0;
    virtual bool Collide(RectangleShape* pOther) = 0;
    virtual bool Collide(ConeShape* pOther) = 0;

    virtual bool Contains(FVec2 point) = 0;

    virtual void Update(yang::TransformComponent* pTransform) = 0;

protected:
    IColor m_color;
public:
    IColor GetColor() const { return m_color; }
    void SetColor(IColor color) { m_color = color; }
};

}