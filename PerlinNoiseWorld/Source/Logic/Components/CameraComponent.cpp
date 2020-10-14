#include "CameraComponent.h"
#include <Logic/Actor/Actor.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/System/CameraSystem.h>
#include <Utils/tinyxml2/tinyxml2.h>
#include <Utils/StringHash.h>
#include <Utils/Logger.h>

#include <algorithm>
#include <cassert>

class SpeedFunction : public CameraComponent::ISpeedFunction
{
public:
    SpeedFunction(float& senseDistance)
        :m_senseDistance(senseDistance)
    {
    }

    float operator()(int distanceFromEdge) const override final
    {
        return 1.f - std::clamp((float)(distanceFromEdge) / m_senseDistance, 0.f, 1.f);
    }
private:
    float& m_senseDistance;
};

using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<yang::IComponent> yang::IComponent::CreateComponent<StringHash32(CameraComponent::GetName())>(yang::Actor* pOwner, std::weak_ptr<CameraSystem> pCameraSystem) 
{
    auto pCameraComponent = std::make_unique<CameraComponent>(pOwner);

    // The engine architecture doesn't allow to have a shared ptrs to the components, because they're stored as unique ptrs in Actors. And right now it is a big pain in the butt to refactor that.
    // So, storing shit in raw pointers
    // TODO(ksizykh) : refactor the engine, so components may be stored as shared/weak pointers
    // TODO(ksizykh) : Remove the camera from camera system after deletion!!!!
    if (auto pCamSys = pCameraSystem.lock(); pCamSys != nullptr)
        pCamSys->RegisterCamera(pCameraComponent.get());

    return pCameraComponent;
};

#pragma warning(pop)

CameraComponent::CameraComponent(yang::Actor* pOwner)
    :IComponent(pOwner, GetName())
    ,m_pSpeedFunction(nullptr)
{
}

bool CameraComponent::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    m_moveSpeed = pData->FloatAttribute("moveSpeed");
    m_senseDistance = pData->FloatAttribute("senseDistance");
    m_startScale = pData->FloatAttribute("startScale", 1.f);

    // Setting up the speed function directly
    // TODO (ksizykh): Figure out how to set it up from data
    m_pSpeedFunction = std::make_unique<SpeedFunction>(m_senseDistance);

    return true;
}

float CameraComponent::CalculateSpeed(int distanceToEdge) const
{
    assert(m_pSpeedFunction);
    return (*m_pSpeedFunction)(distanceToEdge);
}

bool CameraComponent::PostInit()
{
    assert(GetOwner());
    m_pCameraTransform = GetOwner()->GetComponent<yang::TransformComponent>();

    if (!m_pCameraTransform)
    {
        LOG(Error, "Camera doesn't have transform");
        return false;
    }
    m_pCameraTransform->Scale(m_startScale);
    m_pCameraTransform->SetRotationPoint(yang::FVec2(640.f, 360.f));
    return true;
}

void CameraComponent::SetFocusOnActor(yang::Actor* pActor)
{
    assert(pActor);

    yang::TransformComponent* pActorTransform = pActor->GetComponent<yang::TransformComponent>();

    if(!pActorTransform)
    {
        LOG(Error, "Trying to set focus on an actor that doesn't have a transform");
        return;
    }

    SetFocusOnLocation(pActorTransform->GetPosition());
}

void CameraComponent::SetFocusOnLocation(yang::FVec2 location)
{
    assert(m_pCameraTransform);

    m_pCameraTransform->SetPosition(location);
}

void CameraComponent::MoveCamera(yang::FVec2 delta)
{
    assert(m_pCameraTransform);

    yang::Matrix m;
    //m.Rotate(-m_pCameraTransform->GetRotation());
    auto newDelta = m.TransformPoint(delta);

    m_pCameraTransform->Move(newDelta);
}

yang::Matrix CameraComponent::GetCameraTransform() const
{
    assert(m_pCameraTransform);
    return m_pCameraTransform->GetCurrentTransform();
}

float CameraComponent::GetCameraRotation() const
{
    assert(m_pCameraTransform);
    return m_pCameraTransform->GetRotation();
}

yang::FVec2 CameraComponent::GetCameraScaleFactors() const
{
    assert(m_pCameraTransform);
    return m_pCameraTransform->GetScaleFactors();
}