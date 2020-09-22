#include "MoveComponent.h"
#include <Utils/Logger.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Components/Physics/Box2DPhysicsComponent.h>
#include <cassert>

#include <Logic/Physics/Box2DAdapters.h>

#include <Logic/Scripting/LuaManager.h>

using yang::MoveComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> yang::IComponent::CreateComponent<StringHash32(MoveComponent::GetName())>(Actor* pOwner) { return std::make_unique<MoveComponent>(pOwner); };

#pragma warning(pop)

MoveComponent::MoveComponent(yang::Actor* pOwner)
    :IComponent(pOwner, GetName())
    ,m_acceleration(FVec2(0,0))
    ,m_velocity(FVec2(0,0))
    ,m_maxSpeed(0)
{
	
}

MoveComponent::~MoveComponent()
{
	
}

bool yang::MoveComponent::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    m_maxSpeed = pData->FloatAttribute("maxSpeed");

    XMLElement* pVelocity = pData->FirstChildElement("Velocity");
    if (pVelocity)
    {
        m_velocity.x = pVelocity->FloatAttribute("x");
        m_velocity.y = pVelocity->FloatAttribute("y");
    }

    XMLElement* pAcceleration = pData->FirstChildElement("Acceleration");
    if (pAcceleration)
    {
        m_acceleration.x = pAcceleration->FloatAttribute("x");
        m_acceleration.y = pAcceleration->FloatAttribute("y");
    }

    return true;
}

void yang::MoveComponent::RegisterToLua(const LuaManager& manager)
{
	manager.ExposeToLua("GetSpeed", &MoveComponent::GetSpeed);
	manager.ExposeToLua("SetSpeed", &MoveComponent::SetSpeed);
    manager.ExposeToLua("SetVelocity", &MoveComponent::SetVelocity);
    manager.ExposeToLua("GetVelocity", &MoveComponent::GetVelocity);
	manager.ExposeToLua("RotateDirection", &MoveComponent::RotateVelocity);
    manager.ExposeToLua("GetAcceleration", &MoveComponent::GetAcceleration);
    manager.ExposeToLua("SetAcceleration", &MoveComponent::SetAcceleration);
}

void yang::MoveComponent::SetVelocity(FVec2 velocity)
{
    m_velocity = velocity;

    if (m_velocity.SqrdLength() > m_maxSpeed* m_maxSpeed)
        SetSpeed(m_maxSpeed);
}

void yang::MoveComponent::SetSpeed(float speed)
{
    m_velocity.Normalize();
    m_velocity *= (speed > m_maxSpeed ? m_maxSpeed : speed);
}

void yang::MoveComponent::RotateVelocity(float angle)
{
	m_velocity.Rotate(angle);
}

void yang::MoveComponent::RotateAcceleration(float angle)
{
    m_acceleration.Rotate(angle);
}
