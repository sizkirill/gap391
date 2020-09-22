#pragma once

namespace yang
{
class ColliderComponent;

class ICollisionCallback
{
public:
    virtual ~ICollisionCallback() = default;
    virtual void OnCollisionStart(ColliderComponent* pFirst, ColliderComponent* pSecond) = 0;
    virtual void OnCollisionEnd(ColliderComponent* pFirst, ColliderComponent* pSecond) = 0;
    virtual void UpdateCollision(ColliderComponent* pFirst, ColliderComponent* pSecond, float deltaSeconds) = 0;
};
}