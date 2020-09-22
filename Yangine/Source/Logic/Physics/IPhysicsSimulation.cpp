#include "IPhysicsSimulation.h"
#include <Logic/Physics/Box2DPhysics.h>

using yang::IPhysicsSimulation;

IPhysicsSimulation::IPhysicsSimulation()
{
	
}

IPhysicsSimulation::~IPhysicsSimulation()
{
	
}

std::unique_ptr<IPhysicsSimulation> yang::IPhysicsSimulation::Create(FVec2 gravity)
{
    return std::make_unique<Box2DPhysics>(gravity);
}
