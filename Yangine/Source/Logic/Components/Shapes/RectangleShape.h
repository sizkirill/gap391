#pragma once
#include "../IComponent.h"
#include <Utils/Vector2.h>
#include <Utils/Color.h>

namespace yang
{
    class TransformComponent;

class RectangleShape : public IComponent
{
public:
    RectangleShape(Actor* pOwner);

    ~RectangleShape() = default;

    static constexpr const char* GetName() { return "RectangleShape"; }

    virtual bool Init(tinyxml2::XMLElement* pData) override final;
    virtual bool PostInit() override final;
    virtual bool Render(IGraphics* pGraphics) override final;

private:
    FVec2 m_dimensions;
    IColor m_color;
    TransformComponent* m_pActorTransform;
};
}