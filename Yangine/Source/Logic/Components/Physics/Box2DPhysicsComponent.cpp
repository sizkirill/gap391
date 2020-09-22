#include "Box2DPhysicsComponent.h"

#include <Box2D/Box2D.h>

#include <Utils/Logger.h>
#include <Utils/TinyXml2/tinyxml2.h>

#include <Logic/Components/TransformComponent.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Physics/Box2DPhysics.h>

using yang::Box2DPhysicsComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> yang::IComponent::CreateComponent<StringHash32(Box2DPhysicsComponent::GetName())>(Actor* pOwner, yang::IPhysicsSimulation* pPhysics) 
{ 
    return std::make_unique<Box2DPhysicsComponent>(pOwner, pPhysics); 
};

#pragma warning(pop)

Box2DPhysicsComponent::~Box2DPhysicsComponent()
{
    if (m_pBody && m_pWorld)
    {
        // All fixtures attached to this body will be implicitly deleted with a body (see Box2D documentation)
        m_pWorld->DestroyBody(m_pBody);
    }
}

yang::Box2DPhysicsComponent::Box2DPhysicsComponent(yang::Actor* pOwner, IPhysicsSimulation* pPhysics)
    : IPhysicsComponent(pOwner, GetName())
    , m_pBody(nullptr)
    , m_pFixture(nullptr)
    , m_pWorld(static_cast<Box2DPhysics*>(pPhysics)->GetWorld())
{
}

bool yang::Box2DPhysicsComponent::Init(tinyxml2::XMLElement* pData)
{
    if (!m_pWorld)
    {
        LOG(Error, "Box2D world doesn't exist");
        return false;
    }

    using namespace tinyxml2;

    // Static bodies only support box collisions at the moment
    XMLElement* pBox = pData->FirstChildElement("Box");
    bool isSensor = pData->Attribute("isSensor");

    // Defaulted to 0.0
    float gravityScale = pData->FloatAttribute("gravityScale");

    const char* pBodyType = pData->Attribute("type");

    if (pBodyType == nullptr)
    {
        LOG(Warning, "No body type found in PhysicsComponent.  Defaulting to static");
        pBodyType = "static";
    }
    
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2PolygonShape polyShape;

    std::string bodyType = pBodyType;

    if (bodyType == "static")
        bodyDef.type = b2BodyType::b2_staticBody;
    else if (bodyType == "dynamic")
        bodyDef.type = b2BodyType::b2_dynamicBody;
    else if (bodyType == "kinematic")
        bodyDef.type = b2BodyType::b2_kinematicBody;
    else
    {
        LOG(Error, "Incorrect body type for PhysicsComponent: %s", bodyType.c_str());
        return false;
    }

    if (pBox)
    {
        f32 width = pBox->FloatAttribute("width");
        f32 height = pBox->FloatAttribute("height");

        bodyDef.gravityScale = gravityScale;

        polyShape.SetAsBox(width / 2.0f, height / 2.0f);
        fixtureDef.shape = &polyShape;
        fixtureDef.isSensor = isSensor;

        m_pBody = m_pWorld->CreateBody(&bodyDef);
        m_pBody->SetUserData(GetOwner());
        m_pFixture = m_pBody->CreateFixture(&fixtureDef);

        return true;
    }

    return false;
}

bool yang::Box2DPhysicsComponent::PostInit()
{
    m_pTransform = GetOwner()->GetComponent<TransformComponent>();
    if (!m_pTransform)
    {
        LOG(Error, "Physics component requires transform");
        return false;
    }

    m_pBody->SetTransform(Box2DPhysics::ConvertFVec(m_pTransform->GetPosition()), m_pBody->GetAngle());
    
    return true;
}

void yang::Box2DPhysicsComponent::Update(float deltaSeconds)
{
    if (m_pBody->GetType() == b2BodyType::b2_dynamicBody)
    {
        m_pTransform->SetPosition(Box2DPhysics::ConvertB2Vec(m_pBody->GetPosition()));
    }
}

void yang::Box2DPhysicsComponent::SetLinearVelocity(FVec2 velocity)
{
    if (m_pBody)
    {
        m_pBody->SetLinearVelocity(Box2DPhysics::ConvertFVec(velocity));
    }
}

void yang::Box2DPhysicsComponent::ApplyLinearImpulseToCenter(FVec2 impulse)
{
    if (m_pBody)
    {
        m_pBody->ApplyLinearImpulseToCenter(Box2DPhysics::ConvertFVec(impulse), true /* ? */);
    }
}
