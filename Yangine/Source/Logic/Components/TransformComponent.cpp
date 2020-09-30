#include "TransformComponent.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Utils/Logger.h>

#include <Logic/Actor/Actor.h>
#include <Logic/Scripting/LuaManager.h>
#include <Logic/Components/SpriteComponent.h>
#include <Logic/Components/TextComponent.h>

#include <Utils/Random.h>
#include <cassert>

using yang::TransformComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> yang::IComponent::CreateComponent<StringHash32(TransformComponent::GetName())>(Actor* pOwner) { return std::make_unique<TransformComponent>(pOwner); };

#pragma warning(pop)

TransformComponent::TransformComponent(yang::Actor* pOwner)
    :IComponent(pOwner, GetName())
	,m_transformType(TransformType::kWorld)
	,m_transformMatrix()
	,m_transformNeedUpdate(true)
	,m_scale(1.f,1.f)
{
	
}

TransformComponent::~TransformComponent()
{
	
}

bool yang::TransformComponent::Init(tinyxml2::XMLElement* pData)
{
	assert(pData);

	using namespace tinyxml2;
	XMLElement* pPosition = pData->FirstChildElement("Position");

    if (pPosition && pPosition->BoolAttribute("random"))
    {
        auto& rng = XorshiftRNG::GlobalRNG;
        float x = rng.FRand(pPosition->FloatAttribute("minX"), pPosition->FloatAttribute("maxX"));
        float y = rng.FRand(pPosition->FloatAttribute("minY"), pPosition->FloatAttribute("maxY"));
        m_position = FVec2(x, y);
    }
    else
    {
        m_position = FVectorFromXML(pPosition);
    }

	m_rotationPoint = FVectorFromXML(pData->FirstChildElement("RotationPoint"));

    if (pData->BoolAttribute("random"))
    {
        m_rotationAngle = XorshiftRNG::GlobalRNG.FRand(pData->FloatAttribute("min"), pData->FloatAttribute("max"));
    }
    else
    {
        m_rotationAngle = AngleFromXML(pData, "rotationAngle");
    }

    /////////////////////////////////////////////////////////////////////////////////////////////
    /// DEPRECATED. TO BE REMOVED
    /////////////////////////////////////////////////////////////////////////////////////////////
	const char* pTransformType = pData->Attribute("type", "world");
	if (pTransformType)
	{
		std::string transformType = pTransformType;
		if (transformType == "screen")
		{
			m_transformType = TransformType::kScreen;
		}
		else if (transformType == "relative")
		{
			m_transformType = TransformType::kRelative;
		}
		else if (transformType == "world")
		{
			m_transformType = TransformType::kWorld;
		}
	}
    /////////////////////////////////////////////////////////////////////////////////////////////
    /// END OF BLOCK
    /////////////////////////////////////////////////////////////////////////////////////////////

    return true;
}

void yang::TransformComponent::Move(float dx, float dy)
{
	Move(FVec2(dx, dy));
}

void yang::TransformComponent::Move(FVec2 offset)
{
	m_transformNeedUpdate = m_transformNeedUpdate || !Math::IsExtremelyClose(offset.SqrdLength(), 0.f);
	m_position += offset;
}

void yang::TransformComponent::Rotate(float angle)
{
	m_transformNeedUpdate = m_transformNeedUpdate || !Math::IsExtremelyClose(angle, 0.f);
	m_rotationAngle += angle;
	m_rotationAngle = std::fmod(m_rotationAngle, 2 * Math::kPi);
}

void yang::TransformComponent::RegisterToLua(const LuaManager& manager)
{
	manager.ExposeToLua<void(TransformComponent::*)(float, float)>("Move", &TransformComponent::Move);
	manager.ExposeToLua("SetPosition", &TransformComponent::SetPosition);
	manager.ExposeToLua<FVec2&(TransformComponent::*)()>("GetPosition", &TransformComponent::GetPosition);
	manager.ExposeToLua("GetDimensions", &TransformComponent::GetDimensions);
	manager.ExposeToLua("RotateTransform", &TransformComponent::Rotate);
}

void yang::TransformComponent::UpdateTransformMatrix()
{
	m_transformMatrix = Matrix();
	m_transformMatrix.Scale(m_scale, m_rotationPoint).Rotate(m_rotationAngle, m_rotationPoint).Translate(m_position);
}

yang::Matrix& yang::TransformComponent::GetCurrentTransform()
{
	if (m_transformNeedUpdate)
	{
		UpdateTransformMatrix();
		m_transformNeedUpdate = false;
	}
	
	return m_transformMatrix;
}

void yang::TransformComponent::SetPosition(FVec2 position)
{
	m_transformNeedUpdate = m_transformNeedUpdate || !(position == m_position);
	m_position = position;
}

yang::FVec2 yang::TransformComponent::GetDimensions() const
{
	auto pSpriteComp = GetOwner()->GetComponent<SpriteComponent>();
	auto pTextComp = GetOwner()->GetComponent<TextComponent>();

	FVec2 dimensions{ 0,0 };

	if (pSpriteComp)
	{
		dimensions = pSpriteComp->GetDimensions();
	}

	if (pTextComp)
	{
		FVec2 textRelPos = pTextComp->GetRelativePosition();
		IVec2 textTextureDimensions = pTextComp->GetTextureDimensions();

		if (textRelPos.x < 0)
			dimensions.x -= textRelPos.x;

		if (textRelPos.y < 0)
			dimensions.y -= textRelPos.y;

		// These two look weird. Think & test
		if (textTextureDimensions.x + textRelPos.x > dimensions.x)
			dimensions.x = textTextureDimensions.x + textRelPos.x;

		if (textTextureDimensions.y + textRelPos.y > dimensions.y)
			dimensions.y = textTextureDimensions.y + textRelPos.y;

	}

	return dimensions;
}

void yang::TransformComponent::SetRotation(float angle)
{
	m_transformNeedUpdate = m_transformNeedUpdate || !Math::IsExtremelyClose(angle, m_rotationAngle);
    m_rotationAngle = std::fmod(angle + 2 * Math::kPi, 2 * Math::kPi);
}

void yang::TransformComponent::Scale(float amount)
{
	m_transformNeedUpdate = m_transformNeedUpdate || !Math::IsExtremelyClose(amount,1.f);

	m_scale *= amount;
}

void yang::TransformComponent::Scale(FVec2 amount)
{
	m_transformNeedUpdate = m_transformNeedUpdate || !(Math::IsExtremelyClose(amount.x, 1.f) && Math::IsExtremelyClose(amount.y,1.f));

	m_scale.x *= amount.x;
	m_scale.y *= amount.y;
}