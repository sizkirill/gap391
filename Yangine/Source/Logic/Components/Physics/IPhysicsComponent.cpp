#include "IPhysicsComponent.h"

using yang::IPhysicsComponent;

IPhysicsComponent::IPhysicsComponent(yang::Actor* pOwner, const char* name)
    :IComponent(pOwner, name)
{
	
}

IPhysicsComponent::~IPhysicsComponent()
{
	
}

void yang::IPhysicsComponent::HandleBeginOverlap(Actor* pActor)
{
    if (m_onOverlapBegin)
        m_onOverlapBegin(pActor);
}

void yang::IPhysicsComponent::HandleEndOverlap(Actor* pActor)
{
    if (m_onOverlapEnd)
        m_onOverlapEnd(pActor);
}

void yang::IPhysicsComponent::HandleBeginCollision(Actor* pActor)
{
    if (m_onCollisionBegin)
        m_onCollisionBegin(pActor);
}

void yang::IPhysicsComponent::HandleEndCollision(Actor* pActor)
{
    if (m_onCollisionEnd)
        m_onCollisionEnd (pActor);
}
