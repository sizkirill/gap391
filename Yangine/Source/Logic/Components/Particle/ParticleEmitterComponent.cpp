#include "ParticleEmitterComponent.h"
#include <Utils/tinyxml2/tinyxml2.h>
#include <Utils/StringHash.h>
#include <Utils/Rectangle.h>
#include <Utils/Logger.h>
#include <Utils/XMLHelpers.h>
#include <Utils/Math.h>
#include <Application/Graphics/Textures/Sprite.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Components/TransformComponent.h>

using yang::ParticleEmitterComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> yang::IComponent::CreateComponent<StringHash32(ParticleEmitterComponent::GetName())>(yang::Actor* pOwner) { return std::make_unique<ParticleEmitterComponent>(pOwner); };

#pragma warning(pop)

ParticleEmitterComponent::ParticleEmitterComponent(yang::Actor* pOwner)
    :IComponent(pOwner, GetName())
{
	
}

bool ParticleEmitterComponent::Init(tinyxml2::XMLElement* pData)
{
	using namespace tinyxml2;
	assert(pData);

	m_centerOffset = yang::FVectorFromXML(pData->FirstChildElement("CenterOffset"));
	m_size = yang::FVectorFromXML(pData->FirstChildElement("ParticleSize"), { 1.f,1.f });
	m_count = pData->UnsignedAttribute("count", 1u);
	m_radiusSqrd = pData->FloatAttribute("radius", std::numeric_limits<float>::infinity());
	m_radiusSqrd *= m_radiusSqrd;
	
	if (const XMLAttribute* pSeed = pData->FindAttribute("rngSeed"); pSeed != nullptr)
	{
		m_rngDevice = yang::XorshiftRNG(pSeed->Unsigned64Value());
	}

	if (XMLElement* pColor = pData->FirstChildElement("Color"); pColor != nullptr)
	{
		m_drawable = yang::IColorFromXML(pColor);
	}

	if (XMLElement* pSpriteElement = pData->FirstChildElement("Sprite"); pSpriteElement != nullptr)
	{
		auto pSprite = std::make_shared<Sprite>();
		pSprite->Init(pSpriteElement);

		if (!pSprite)
		{
			LOG(Error, "Sprite failed to initialize");
			return false;
		}

		m_drawable = pSprite;
	}

	m_lifetime = pData->FloatAttribute("lifetime", std::numeric_limits<float>::infinity());
	m_speedRange = FVectorFromXML(pData->FirstChildElement("SpeedRange"), { 0.f, 10.f });
	m_initialAngleRange = FVectorFromXML(pData->FirstChildElement("AngleRange"), { 0.f, 360.f });

	m_particles.reserve(m_count);
	
	for (size_t i = 0; i < m_count; ++i)
	{
		float randomAngle = m_rngDevice.FRand(m_initialAngleRange.x, m_initialAngleRange.y);
		FVec2 randomVelocity = FVec2::FromAngle(Math::ToRadians(randomAngle));
		m_particles.push_back({ {0,0}, randomVelocity * m_rngDevice.FRand(m_speedRange.x, m_speedRange.y)});
	}

    return true;
}

void yang::ParticleEmitterComponent::Update(float deltaSeconds)
{
	// TODO(ksizykh, yukishi): if particle emitter component's lifetime has passed, we need to either delete it and that's all
	//		or just set some flag so it won't render. May be it can be reused later? Think about it 
	if (m_lifetime < 0)
	{
		return;
	}

	for (auto& particle : m_particles)
	{
		// Update each particle's position.
		particle.m_positionOffset += particle.m_velocity * deltaSeconds;

		// If Particle is past the edge of the system radius recycle it.
		// TODO(ksizykh, yuki): think how is it better - to have a particle system that has particle lifetime, or only maximum radius,
		//	or conditionally both?
		if (particle.m_positionOffset.SqrdLength() > m_radiusSqrd)
		{
			particle.m_positionOffset = { 0,0 };
		}
	}

	m_lifetime -= deltaSeconds;
}

bool yang::ParticleEmitterComponent::Render(yang::IGraphics* pGraphics)
{
	bool success = true;
	yang::FVec2 transformPosition = m_pOwnerTransform->GetPosition();
	yang::FVec2 scaleFactors = m_pOwnerTransform->GetScaleFactors();

	// may be needed later in refactoring?
	//float rotationAngle = m_pOwnerTransform->GetRotation();

	for (const auto& particle : m_particles)
	{
		Vector2<float> position = particle.m_positionOffset;

		yang::FRect particleTransform;
		particleTransform.x = transformPosition.x + m_centerOffset.x + position.x;
		particleTransform.y = transformPosition.y + m_centerOffset.y + position.y;
		particleTransform.height = m_size.y * scaleFactors.y;	// Particle's height
		particleTransform.width = m_size.x * scaleFactors.x;	// Particle's width

		success = success && std::visit([pGraphics, particleTransform](auto&& drawable) -> bool
			{
				using Type = std::decay_t<decltype(drawable)>;
				if constexpr (std::is_same_v<Type, std::shared_ptr<yang::Sprite>>)
				{
					return pGraphics->DrawSprite(drawable, particleTransform);
				}
				else if constexpr (std::is_same_v<Type, yang::IColor>)
				{
					return pGraphics->FillRect(particleTransform, drawable);
				}
			}, m_drawable);
	}

	return success;
}

bool yang::ParticleEmitterComponent::PostInit()
{
	assert(GetOwner());

	m_pOwnerTransform = GetOwner()->GetComponent<yang::TransformComponent>();

	if (!m_pOwnerTransform)
	{
		LOG(Error, "Particle component requires TransformComponent");
		return false;
	}
	
	return true;
}
