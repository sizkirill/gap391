#pragma once

#include "CircleShape.h"
#include <Utils/StringHash.h>

namespace tinyxml2
{
    class XMLElement;
}

namespace yang
{

class ConeShape : public CircleShape
{
public:
    ConeShape(FVec2 center, float radius, float fov, float orientation, IColor color = {0,0,0,0});
    ConeShape(tinyxml2::XMLElement* pData);

    virtual bool Init(tinyxml2::XMLElement* pData) override final;
    virtual bool Render(IGraphics* pGraphics) override final;
    virtual bool DebugDraw(IGraphics* pGraphics) override final;
    virtual bool Collide(IShape* pOther) override final;
    virtual bool Collide(CircleShape* pOther) override final;
    virtual bool Collide(RectangleShape* pOther) override final;
    virtual bool Collide(ConeShape* pOther) override final;

    virtual bool Contains(FVec2 point) override final;

    virtual void Update(TransformComponent* pTransform) override final;

    static constexpr const char* GetName() { return "ConeShape"; }
    static constexpr uint32_t GetHashName() { return StringHash32(GetName()); }
private:
    float m_fieldOfViewAngle;
    float m_orientationAngle;
public:
    float GetOrientation() const { return m_orientationAngle; }
    float GetAngle() const { return m_fieldOfViewAngle; }

    void SetOrientation(float orientation);
    void SetAngle(float angle);
};
}