#include "KinematicComponent.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/XMLHelpers.h>
#include <Utils/StringHash.h>
#include <Utils/Logger.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Actor/Actor.h>

#include <cassert>

using namespace yang;

template<>
std::unique_ptr<IComponent> IComponent::CreateComponent<StringHash32(KinematicComponent::GetName())>(yang::Actor* pOwner) 
{ 
    return std::make_unique<KinematicComponent>(pOwner); 
}

KinematicComponent::KinematicComponent(yang::Actor* pOwner)
    :IComponent(pOwner, GetName())
{
}

void KinematicComponent::Accelerate(float deltaTime, yang::FVec2 linearAcceleration, float angularAcceleration)
{
    linearAcceleration.ClampMagnitude(m_maxAcceleration);
    m_acceleration = linearAcceleration;
    //m_angularAcceleration = std::clamp(angularAcceleration, -m_maxAngularAcceleration, m_maxAngularAcceleration);

    m_velocity += m_acceleration * deltaTime;
    m_velocity.ClampMagnitude(m_maxSpeed);

    //m_angularVelocity += m_angularAcceleration * deltaTime;
    //m_angularVelocity = std::clamp(m_angularVelocity, -m_maxAngularVelocity, m_maxAngularVelocity);
}

void yang::KinematicComponent::Rotate(float deltaTime)
{
    if (Math::IsExtremelyClose(Math::AngleDiff(m_targetOrientation, m_pTransform->GetRotation()), 0.f, 0.05f))
    {
        m_angularVelocity = 0;
        return;
    }

    float thisAngle = m_pTransform->GetRotation();
    float neededAngle = m_targetOrientation;

    if (Math::AngleDiff(neededAngle, std::fmod(thisAngle + 0.01f, 2 * Math::kPi)) < Math::AngleDiff(neededAngle, thisAngle))
    {
        m_angularVelocity += m_maxAngularAcceleration * deltaTime;
    }
    else
    {
        m_angularVelocity -= m_maxAngularAcceleration * deltaTime;
    }


    //m_angularVelocity += m_maxAngularAcceleration * deltaTime;
    m_pTransform->Rotate(m_angularVelocity * deltaTime);
}

void KinematicComponent::Stop()
{
    m_acceleration = FVec2(0.f, 0.f);
    m_velocity = FVec2(0.f, 0.f);
}

bool KinematicComponent::Init(tinyxml2::XMLElement* pData)
{
    assert(pData);
    using namespace tinyxml2;

    XMLElement* pVelocity = pData->FirstChildElement("Velocity");
    m_velocity = yang::FVectorFromXML(pVelocity);

    XMLElement* pAcceleration = pData->FirstChildElement("Acceleration");
    m_acceleration = yang::FVectorFromXML(pAcceleration);

    m_maxSpeed = pData->FloatAttribute("maxSpeed");
    m_maxAcceleration = pData->FloatAttribute("maxAcceleration");
    m_angularVelocity = yang::AngleFromXML(pData, "angularVelocity");
    m_maxAngularVelocity = yang::AngleFromXML(pData, "maxAngularVelocity");
    m_angularAcceleration = yang::AngleFromXML(pData, "angularAcceleration");
    m_maxAngularAcceleration = yang::AngleFromXML(pData, "maxAngularAcceleration");
    
    return true;
}

bool KinematicComponent::PostInit()
{
    m_pTransform = GetOwner()->GetComponent<yang::TransformComponent>();

    if (!m_pTransform)
    {
        LOG(Error, "Actor doesn't have transform component, unable to initialize KinematicComponent");
        return false;
    }
    m_targetOrientation = m_pTransform->GetRotation();
    return true;
}

void KinematicComponent::SetOrientationFromVelocity()
{
    m_pTransform->SetRotation(std::atan2(m_velocity.y, m_velocity.x));
}
