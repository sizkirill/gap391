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
    using namespace tinyxml2;

    const char* pSourcePath = pData->Attribute("Src");
    if (pSourcePath)
    {
        m_pTexture = ResourceCache::Get()->Load<ITexture>(pSourcePath);
    }
    else
    {
        LOG(Error, "Sprite Component needs to have source path to texture");
        return false;
    }

    XMLElement* pTint = pData->FirstChildElement("Tint");
    if (pTint)
    {
		IColor tint(pTint->IntAttribute("r"),
					pTint->IntAttribute("g"),
					pTint->IntAttribute("b"),
					pTint->IntAttribute("a"));

        m_pTexture->SetTint(tint);
        m_pTexture->SetAlpha(tint.Alpha());
    }

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

    XMLElement* pSourceRect = pData->FirstChildElement("SourceRect");
    if (pSourceRect)
    {
        m_textureSourceRect.x = pSourceRect->IntAttribute("x");
        m_textureSourceRect.y = pSourceRect->IntAttribute("y");
        m_textureSourceRect.width = pSourceRect->IntAttribute("width");
        m_textureSourceRect.height = pSourceRect->IntAttribute("height");
    }
    else
    {
        LOG(Warning, "No source rect for sprite specified. Using whole texture");
    }

    return true;
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
    const FVec2& position = m_pTransform->GetPosition();
    // m_textureDrawParams.m_pointToRotate = m_pTransform->GetRotationPoint();
    // m_textureDrawParams.m_angle = m_pTransform->GetRotation();
    IRect dest = IRect{(i32)position.x - (m_spriteDimensions.x / 2), 
        (i32)position.y - (m_spriteDimensions.y / 2),
        m_spriteDimensions.x, 
        m_spriteDimensions.y };
    return pGraphics->DrawTexture(m_pTexture.get(), m_textureSourceRect, dest, m_textureDrawParams);
}

void yang::SpriteComponent::RegisterToLua(const LuaManager& luaManager)
{
	luaManager.ExposeToLua("SetRotationAngle", &SpriteComponent::SetRotationAngle);
	luaManager.ExposeToLua("GetRotationAngle", &SpriteComponent::GetRotationAngle);
	luaManager.ExposeToLua("SetRotationPoint", &SpriteComponent::SetRotationPoint);
	luaManager.ExposeToLua("GetRotationPoint", &SpriteComponent::GetRotationPoint);

	// TODO: Reconsider the naming?
	luaManager.ExposeToLua("SetSpriteDirection", &SpriteComponent::SetDirection);

	// TODO: What about enums? cast them to int?
	//luaManager.ExposeToLua("SetTextureFlip", &SpriteComponent::SetFlip);
	//luaManager.ExposeToLua("GetTextureFlip", &SpriteComponent::GetFlip);

	// TODO: Think about exposing texture source rect & dimensions
}

void yang::SpriteComponent::SetRotationAngle(float angle)
{
	m_textureDrawParams.m_angle = angle;
    //m_pTransform->SetRotation(angle);
}

float yang::SpriteComponent::GetRotationAngle() const
{
	return m_textureDrawParams.m_angle;
}

void yang::SpriteComponent::SetRotationPoint(IVec2 point)
{
	m_textureDrawParams.m_pointToRotate = point;
    //m_pTransform->SetRotationPoint(point);
}

std::optional<yang::IVec2> yang::SpriteComponent::GetRotationPoint() const
{
	return m_textureDrawParams.m_pointToRotate;
}

void yang::SpriteComponent::SetFlip(FlipDirection flip)
{
	m_textureDrawParams.m_flip = flip;
}

yang::FlipDirection yang::SpriteComponent::GetFlip() const
{
	return m_textureDrawParams.m_flip;
}

void yang::SpriteComponent::SetDirection(FVec2 fromPoint, FVec2 toPoint)
{
	SetRotationPoint(IVec2(m_spriteDimensions.x / 2, m_spriteDimensions.y / 2));

	LOG_ONCE(TODO, RotatingOffset, "Think why it offsets by pi/2 when rotating...");
	SetRotationAngle(Math::ToDegrees(std::atan2f(toPoint.y - fromPoint.y, toPoint.x - fromPoint.x) + Math::kPi / 2));
}
