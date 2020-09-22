#include "ColliderComponent.h"
#include <Utils/tinyxml2/tinyxml2.h>
#include <Utils/StringHash.h>
#include <Logic/Collisions/CollisionSystem.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Actor/Actor.h>
#include <cassert>

using yang::ColliderComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> yang::IComponent::CreateComponent<StringHash32(ColliderComponent::GetName())>(yang::Actor* pOwner, yang::CollisionSystem* pCollisionSystem) 
{ 
    return std::make_unique<ColliderComponent>(pOwner, pCollisionSystem); 
};

#pragma warning(pop)

ColliderComponent::ColliderComponent(yang::Actor* pOwner, yang::CollisionSystem* pCollisionSystem)
    :IComponent(pOwner, GetName())
    ,m_pColliderShape(nullptr)
    ,m_pCollisionCallback(nullptr)
    ,m_type(Type::kCollider)
{
    pCollisionSystem->RegisterCollider(0, this);
}

bool ColliderComponent::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    XMLElement* pShape = pData->FirstChildElement("Shape");

    if (!pShape)
    {
        return false;
    }

    const char* pName = pShape->Attribute("name");
    if (!pName)
    {
        return false;
    }

    m_pColliderShape = IShape::CreateShape(pName, pShape);

    if (!m_pColliderShape)
    {
        return false;
    }

    return true;
}

bool yang::ColliderComponent::PostInit()
{
    m_pTransform = GetOwner()->GetComponent<TransformComponent>();
    return m_pTransform != nullptr;
}

bool yang::ColliderComponent::Collide(ColliderComponent* pOther)
{
    assert(pOther);
    return m_pColliderShape->Collide(pOther->GetShape());
}

bool yang::ColliderComponent::Render(IGraphics* pGraphics)
{
    // DEBUG
    return m_pColliderShape->DebugDraw(pGraphics);
}

void yang::ColliderComponent::Update(float deltaSeconds)
{
    m_pColliderShape->Update(m_pTransform);
}

void yang::ColliderComponent::OnCollisionStart(ColliderComponent* pOther)
{
    // DEBUG
    m_pColliderShape->SetColor(IColor(255, 0, 0, 255));

    if (m_pCollisionCallback)
    {
        m_pCollisionCallback->OnCollisionStart(this, pOther);
    }
}

void yang::ColliderComponent::OnCollisionEnd(ColliderComponent* pOther)
{
    // DEBUG
    m_pColliderShape->SetColor(IColor(0, 0, 255, 255));

    if (m_pCollisionCallback)
    {
        m_pCollisionCallback->OnCollisionEnd(this, pOther);
    }
}

void yang::ColliderComponent::UpdateCollision(ColliderComponent* pOther, float deltaSeconds)
{
    // Again, DEBUG stuff. Collisions should work properly, but, debug color highlighting doesnt work properly
    //      unless we're updating the color. (Because it changes at OnCollisionStart & OnCollisionEnd =>
    //      after one collision ended, it changes color to blue and then it will be red only after another collision started.
    //      After last OnCollisionEnd it should become blue.
    m_pColliderShape->SetColor(IColor(255, 0, 0, 255));

    if (m_pCollisionCallback)
    {
        m_pCollisionCallback->UpdateCollision(this, pOther, deltaSeconds);
    }
}

yang::ColliderComponent::ColliderComponent(yang::Actor* pOwner, CollisionSystem* pCollisionSystem, Type type, const char* pName)
    :IComponent(pOwner, pName)
    , m_pColliderShape(nullptr)
    ,m_type(type)
    , m_pCollisionCallback(nullptr)
    ,m_pTransform(nullptr)
{
    pCollisionSystem->RegisterCollider(0, this);
}
