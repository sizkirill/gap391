#include "Collision.h"
#include <Logic/Components/Colliders/ColliderComponent.h>

yang::Collision::Collision(ColliderComponent* pFirst, ColliderComponent* pSecond)
    :m_pFirst(pFirst)
    ,m_pSecond(pSecond)
{
}

void yang::Collision::OnCollisionEnter()
{
    m_pFirst->OnCollisionStart(m_pSecond);
    m_pSecond->OnCollisionStart(m_pFirst);
}

void yang::Collision::OnCollisionExit()
{
    m_pFirst->OnCollisionEnd(m_pSecond);
    m_pSecond->OnCollisionEnd(m_pFirst);
}

void yang::Collision::Update(float deltaSeconds)
{
    m_pFirst->UpdateCollision(m_pSecond, deltaSeconds);
    m_pSecond->UpdateCollision(m_pFirst, deltaSeconds);
}
