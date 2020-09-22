#pragma once
#include <utility>

namespace yang
{ 
class ColliderComponent;

class Collision
{
public:
    Collision(ColliderComponent* pFirst, ColliderComponent* pSecond);
    void OnCollisionEnter();
    void OnCollisionExit();
    void Update(float deltaSeconds);

private:
    ColliderComponent* m_pFirst;
    ColliderComponent* m_pSecond;
public:
    std::pair<ColliderComponent*, ColliderComponent*> GetCollisionPair() { return std::make_pair(m_pFirst, m_pSecond); }
};
}