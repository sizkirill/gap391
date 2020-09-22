#include "IContactListener.h"

#include <Box2D/Box2D.h>

#include <Utils/Logger.h>
#include <Logic/Actor/Actor.h>
#include <Logic/IGameLayer.h>

using yang::IContactListener;

IContactListener::IContactListener(yang::IGameLayer* pGameLayer)
    :m_pGameLayer(pGameLayer)
{
	
}

IContactListener::~IContactListener()
{
	
}

void yang::IContactListener::BeginContact(b2Contact* pContact)
{
    HandleContact(pContact, ContactState::kBegin);
}

void yang::IContactListener::EndContact(b2Contact* pContact)
{
    HandleContact(pContact, ContactState::kEnd);
}

//void yang::IContactListener::PreSolve(b2Contact* pContact, const b2Manifold* pOldManifold)
//{
//    //
//}
//
//void yang::IContactListener::PostSolve(b2Contact* pContact, const b2ContactImpulse* pImpulse)
//{
//    //
//}

void yang::IContactListener::HandleContact(b2Contact* pContact, ContactState type)
{
    b2Fixture* pFixtureA = pContact->GetFixtureA();
    b2Fixture* pFixtureB = pContact->GetFixtureB();

    b2Body* pBodyA = pFixtureA->GetBody();
    b2Body* pBodyB = pFixtureB->GetBody();

    Actor* pActorA = static_cast<Actor*>(pBodyA->GetUserData());
    Actor* pActorB = static_cast<Actor*>(pBodyB->GetUserData());

    // Unowned fixtures??
    if (!pActorA || !pActorB)
        return;

    bool isOverlap = pFixtureA->IsSensor() || pFixtureB->IsSensor();
    
    LOG(Info, "%s and %s %s %s!", pActorA->GetTag().c_str(), pActorB->GetTag().c_str(), type == ContactState::kBegin ? "started" : "stopped", isOverlap ? "overlapping" : "colliding");

    if (m_pGameLayer)
    {
        m_pGameLayer->NotifyContact(pActorA, pActorB, isOverlap, type);
    }
    else
    {
        LOG(Error, "m_pGameLayer was nullptr");
    }

}
