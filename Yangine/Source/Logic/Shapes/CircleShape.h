#pragma once

#include "IShape.h"
#include <Utils/Vector2.h>
#include <Utils/StringHash.h>

namespace tinyxml2
{
    class XMLElement;
}

namespace yang
{

class CircleShape : public IShape
{
public:
    CircleShape(FVec2 center, float radius, IColor color = {0,0,0,0});
    CircleShape(tinyxml2::XMLElement* pData);

    virtual ~CircleShape() = default;
    virtual bool Init(tinyxml2::XMLElement* pData) override;
    virtual bool Render(IGraphics* pGraphics) override;
    virtual bool DebugDraw(IGraphics* pGraphics) override;
    virtual bool Collide(IShape* pOther) override;
    virtual bool Collide(CircleShape* pOther) override;
    virtual bool Collide(RectangleShape* pOther) override;
    virtual bool Collide(ConeShape* pOther) override;

    virtual bool Contains(FVec2 point) override;

    virtual void Update(yang::TransformComponent* pTransform) override;

    static constexpr const char* GetName() { return "CircleShape"; }
    static constexpr uint32_t GetHashName() { return StringHash32(GetName()); }
protected:
    FVec2 m_localCenter;
    FVec2 m_center;
    float m_radius;
public:
    FVec2 GetCenter() const { return m_localCenter + m_center; }
    float GetRadiusSqrd() const { return m_radius; }

    void SetCenter(FVec2 center) { m_center = center; }
    void SetRadius(float radius) { m_radius = radius; }
};
}