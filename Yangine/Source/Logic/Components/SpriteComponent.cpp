#include "SpriteComponent.h"
#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/Textures/ITexture.h>
#include <Application/Resources/ResourceCache.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Scripting/LuaManager.h>
#include <cassert>

using yang::SpriteComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> yang::IComponent::CreateComponent<StringHash32(SpriteComponent::GetName())>(Actor* pOwner)
{ 
    return std::make_unique<SpriteComponent>(pOwner); 
};

#pragma warning(pop)

yang::SpriteComponent::SpriteComponent(Actor* pOwner)
    :IComponent(pOwner, GetName())
{
}

SpriteComponent::~SpriteComponent()
{
	
}

bool yang::SpriteComponent::Init(tinyxml2::XMLElement* pData)
{
    assert(pData);
    using namespace tinyxml2;

    m_pSprite = std::make_shared<Sprite>();

    XMLElement* pDimensions = pData->FirstChildElement("Dimensions");
    if (pDimensions)
    {
        m_spriteDimensions.x = pDimensions->IntAttribute("width");
        m_spriteDimensions.y = pDimensions->IntAttribute("height");
    }
    else
    {
        LOG(Warning, "No dimensions for sprite component specified. Using default");
    }

    return m_pSprite->Init(pData);
}

bool yang::SpriteComponent::PostInit()
{
    assert(GetOwner());
    m_pTransform = GetOwner()->GetComponent<TransformComponent>();
    if (!m_pTransform)
    {
        LOG(Error, "Actor doesn't have transform component. Sprite component was unable to init");
        return false;
    }

    return true;
}

bool yang::SpriteComponent::Render(IGraphics* pGraphics)
{
    assert(m_pSprite != nullptr);
    assert(m_pTransform != nullptr);

    const FVec2& position = m_pTransform->GetPosition();
    // m_textureDrawParams.m_pointToRotate = m_pTransform->GetRotationPoint();
    // m_textureDrawParams.m_angle = m_pTransform->GetRotation();
    IRect dest = IRect{(i32)position.x - (m_spriteDimensions.x / 2), 
        (i32)position.y - (m_spriteDimensions.y / 2),
        m_spriteDimensions.x, 
        m_spriteDimensions.y };
    return pGraphics->DrawSprite(m_pSprite, dest);
}

void yang::SpriteComponent::RegisterToLua(const LuaManager& luaManager)
{
	luaManager.ExposeToLua("SetRotationAngle", &SpriteComponent::SetRotationAngle);
	luaManager.ExposeToLua("GetRotationAngle", &SpriteComponent::GetRotationAngle);
	luaManager.ExposeToLua("SetRotationPoint", &SpriteComponent::SetRotationPoint);
	luaManager.ExposeToLua("GetRotationPoint", &SpriteComponent::GetRotationPoint);

	luaManager.ExposeToLua("SetSpriteDirection", &SpriteComponent::SetDirection);
}

void yang::SpriteComponent::SetRotationAngle(float angle)
{
    assert(m_pSprite != nullptr);
	m_pSprite->GetDrawParams().m_angle = angle;
    //m_pTransform->SetRotation(angle);
}

float yang::SpriteComponent::GetRotationAngle() const
{
    assert(m_pSprite != nullptr);
	return m_pSprite->GetDrawParams().m_angle;
}

void yang::SpriteComponent::SetRotationPoint(IVec2 point)
{
    assert(m_pSprite != nullptr);
    m_pSprite->GetDrawParams().m_pointToRotate = point;
    //m_pTransform->SetRotationPoint(point);
}

std::optional<yang::IVec2> yang::SpriteComponent::GetRotationPoint() const
{
    assert(m_pSprite != nullptr);
	return m_pSprite->GetDrawParams().m_pointToRotate;
}

void yang::SpriteComponent::SetFlip(FlipDirection flip)
{
    assert(m_pSprite != nullptr);
    m_pSprite->GetDrawParams().m_flip = flip;
}

yang::FlipDirection yang::SpriteComponent::GetFlip() const
{
    assert(m_pSprite != nullptr);
	return m_pSprite->GetDrawParams().m_flip;
}

void yang::SpriteComponent::SetDirection(FVec2 fromPoint, FVec2 toPoint)
{
	SetRotationPoint(IVec2(m_spriteDimensions.x / 2, m_spriteDimensions.y / 2));
	SetRotationAngle(Math::ToDegrees(std::atan2f(toPoint.y - fromPoint.y, toPoint.x - fromPoint.x) + Math::kPi / 2));
}
