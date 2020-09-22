#include "CollisionSystem.h"
#include <Logic/Components/Colliders/ColliderComponent.h>
#include <Logic/Actor/Actor.h>

void yang::CollisionSystem::RegisterCollider(int layer, ColliderComponent* pComponent)
{
    m_colliders[layer].emplace_back(pComponent);
}

void yang::CollisionSystem::Update(float deltaSeconds)
{
    // Triggering OnEnter on new collisions
    for (Collision* pCollision : m_recentCollisions)
    {
        pCollision->OnCollisionEnter();
    }
    m_recentCollisions.clear();

    // Updating active collisions (including recently added)
    for (auto it = m_activeCollisions.begin(); it != m_activeCollisions.end();)
    {
        auto& [collisionPair, pCollision] = *it;
        pCollision->Update(deltaSeconds);
        if (!collisionPair.first->Collide(collisionPair.second))
        {
            pCollision->OnCollisionExit();
            m_activeCollisionsByActor.erase(collisionPair.first);
            m_activeCollisionsByActor.erase(collisionPair.second);
            it = m_activeCollisions.erase(it);
            continue;
        }
        ++it;
    }

    // Checking for new collisions
    for (auto& [layerId, colliders] : m_colliders)
    {
        // Yeah, straight forward n^2 algorithm
        for (size_t i = 0; i < colliders.size() - 1; ++i)
        {
            for (size_t j = i + 1; j < colliders.size(); ++j)
            {
                if (colliders[i]->GetOwner() == colliders[j]->GetOwner())
                {
                    continue;
                }

                if (colliders[i]->Collide(colliders[j]))
                {
                    if (m_activeCollisions.find(std::make_pair(colliders[i], colliders[j])) == m_activeCollisions.end())
                    {
                        auto [emplacedIt, wasEmplaced] = m_activeCollisions.emplace(std::make_pair(colliders[i], colliders[j]), std::make_unique<Collision>(colliders[i], colliders[j]));
                        m_activeCollisionsByActor[emplacedIt->first.first].emplace_back(emplacedIt->second.get());
                        m_activeCollisionsByActor[emplacedIt->first.second].emplace_back(emplacedIt->second.get());
                        m_recentCollisions.emplace_back(emplacedIt->second.get());
                    }
                }
            }
        }
    }
}

const std::vector<yang::Collision*>& yang::CollisionSystem::GetCollisionsOnCollider(ColliderComponent* pCollider)
{
    return m_activeCollisionsByActor.at(pCollider);
}

void yang::CollisionSystem::ClearCollisionsWithActor(yang::Actor* pActor)
{
    // For each layer
    //      O(l) time complexity , where l is number of layers
    for (auto& [layer, colliders] : m_colliders)
    {
        auto findLambda = [pActor](ColliderComponent* pComponent) {return pComponent->GetOwner() == pActor; };
        // Clear collisions with each found satisfied collider.
        //      Finding all satisfying colliders. Time complexity O(c), where c is number of colliders on that collision layer
        //              + cost of erasing (up to O(c))
        //      Total thus far: O(l * (2 * c))
        auto it = std::find_if(colliders.begin(), colliders.end(), findLambda);
        while (it != colliders.end())
        {
            // O(k^2) - where k is an average number of active collisions on each collider
            ClearCollisionsWithCollider(*it);
            it = colliders.erase(it);
            it = std::find_if(it, colliders.end(), findLambda);
        }
    }

    // Total complexity : O(l * c * k^2). 
    //  k is expected to be very low (how many active collisions can be on actor at the same time, really?), so kind of okayish?
    //  I would consider it to be amortized O(n), where n is total number of registered colliders
}

void yang::CollisionSystem::ClearCollisionsWithCollider(ColliderComponent* pCollider)
{
    auto& collisions = m_activeCollisionsByActor[pCollider];

    // For each active collision of that collider
    //      O(f), where f is number of collisions on our collider
    for (auto pCollision : collisions)
    {
        auto [pFirstCollider, pSecondCollider] = pCollision->GetCollisionPair();

        // just swap for convenience, to know that pFirstCollider is the one we're deleting on
        //      and pSecondCollider is the other one.
        if (pFirstCollider != pCollider)
        {
            std::swap(pFirstCollider, pSecondCollider);
        }

        auto& secondActorCollisions = m_activeCollisionsByActor[pSecondCollider];
        
        // Find that collision in a second collider's active collisions and erase it from there
        //      O(k), where k is a number of active collisions on a second collider
        auto it = std::find(secondActorCollisions.begin(), secondActorCollisions.end(), pCollision);
        it = secondActorCollisions.erase(it);

        // find that collision in all active collisions and erase it.
        //     O(1)
        if (auto it = m_activeCollisions.find(pCollision->GetCollisionPair()); it != m_activeCollisions.end())
        {
            m_activeCollisions.erase(it);
        }
    }
}
