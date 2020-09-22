#pragma once

#include "IShape.h"
#include <Utils/Rectangle.h>
#include <Utils/StringHash.h>
#include <array>

namespace tinyxml2
{
    class XMLElement;
}


namespace yang
{

class RectangleShape : public IShape
{
public:
    RectangleShape(FVec2 center, FVec2 dimensions, IColor color = {0,0,0,0});
    RectangleShape(tinyxml2::XMLElement* pData);
    virtual bool Init(tinyxml2::XMLElement* pData) override final;
    virtual bool Render(IGraphics* pGraphics) override final;
    virtual bool DebugDraw(IGraphics* pGraphics) override final;

    virtual bool Collide(IShape* pOther) override final;
    virtual bool Collide(CircleShape* pOther) override final;
    virtual bool Collide(RectangleShape* pOther) override final;
    virtual bool Collide(ConeShape* pOther) override final;

    virtual bool Contains(FVec2 point) override final;

    virtual void Update(TransformComponent* pTransform) override final;

    std::array<FVec2, 4> GetVertices() const;

    static constexpr const char* GetName() { return "RectangleShape"; }
    static constexpr uint32_t GetHashName() { return StringHash32(GetName()); }
private:
    FVec2 m_localCenter;
    FVec2 m_center;
    FVec2 m_dimensions;

public:
    FVec2 GetCenter() const { return m_localCenter + m_center; }
    FVec2 GetDimensions() const { return m_dimensions; }
    FRect GetRect() const;

    void SetCenter(FVec2 center) { m_center = center; }
    void SetDimensions(FVec2 dimensions) { m_dimensions = dimensions; }
};
}