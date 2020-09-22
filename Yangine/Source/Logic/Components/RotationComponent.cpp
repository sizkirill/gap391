#include "RotationComponent.h"
#include <Utils/TinyXml2/tinyxml2.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Components/TransformComponent.h>

using yang::RotationComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> yang::IComponent::CreateComponent<StringHash32(RotationComponent::GetName())>(Actor* pOwner) { return std::make_unique<RotationComponent>(pOwner); };

#pragma warning(pop)

RotationComponent::RotationComponent(yang::Actor* pOwner)
	:IComponent(pOwner, GetName())
    ,m_rotationPoint(FVec2(0.f,0.f))
    ,m_rotationSpeed(0.f)
    ,m_acceleration(0.f)
    ,m_pActorTransform(nullptr)
{
	
}

RotationComponent::~RotationComponent()
{
	
}

bool yang::RotationComponent::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    m_maxRotationSpeed = pData->FloatAttribute("maxSpeed");
    m_rotationSpeed = pData->FloatAttribute("speed");
    m_acceleration = pData->FloatAttribute("acceleration");

    XMLElement* pRotationPoint = pData->FirstChildElement("RotationPoint");
    if (pRotationPoint)
    {
        m_rotationPoint.x = pRotationPoint->FloatAttribute("x");
        m_rotationPoint.y = pRotationPoint->FloatAttribute("y");
    }

    return true;
}

void yang::RotationComponent::SetRotationSpeed(float degreesPerSec)
{
    if (std::fabs(degreesPerSec) > m_maxRotationSpeed)
    {
        m_rotationSpeed = (degreesPerSec > 0 ? m_maxRotationSpeed : -m_maxRotationSpeed);
    }
    else
    {
        m_rotationSpeed = degreesPerSec;
    }
}
