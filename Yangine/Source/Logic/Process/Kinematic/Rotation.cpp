#include "Rotation.h"
#include <Logic/Actor/Actor.h>
#include <Logic/Components/TransformComponent.h>

yang::RotationProcess::RotationProcess(yang::Actor* pOwner, float executionTime)
    :TimedProcess(pOwner, executionTime, &RotationProcess::Rotate, this)
    ,m_pTransform(nullptr)
{
}

bool yang::RotationProcess::Init()
{
    m_pTransform = GetOwner()->GetComponent<TransformComponent>();
    return true;
}

void yang::RotationProcess::Rotate(float deltaTime)
{
    //float angleAcceleration = 
}
