#include "Box2DPhysics.h"
#include <Logic/Components/Physics/Box2DPhysicsComponent.h>
#include <Logic/Physics/IContactListener.h>

using yang::Box2DPhysics;

Box2DPhysics::Box2DPhysics(yang::FVec2 gravity)
    :m_world(ConvertFVec(gravity))
    ,m_accumulator(0.f)
{
	
}

Box2DPhysics::~Box2DPhysics()
{
	
}

bool yang::Box2DPhysics::Init()
{
    return true;
}

void yang::Box2DPhysics::Update(float deltaSeconds)
{
    m_accumulator += deltaSeconds;

    // Or while?
    if (m_accumulator >= kPhysicsStep)
    { 
        m_accumulator -= kPhysicsStep;
        m_world.Step(kPhysicsStep, kVelocityIterations, kPositionIterations);
    }

}

void yang::Box2DPhysics::SetContactListener(IContactListener* pListener)
{
    m_world.SetContactListener(pListener);
}

b2Vec2 yang::Box2DPhysics::ConvertFVec(FVec2 vec)
{
    return b2Vec2(vec.x / kPixelsPerMeter, vec.y / kPixelsPerMeter);
}

yang::FVec2 yang::Box2DPhysics::ConvertB2Vec(b2Vec2 vec)
{
    return FVec2(vec.x * kPixelsPerMeter, vec.y * kPixelsPerMeter);
}
