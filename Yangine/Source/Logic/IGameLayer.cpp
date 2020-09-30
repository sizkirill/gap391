#include "IGameLayer.h"
#include <Logic/Actor/Actor.h>
#include <Application/ApplicationLayer.h>
#include <Logic/Physics/IContactListener.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Components/SpriteComponent.h>
#include <Logic/Components/MoveComponent.h>
#include <Logic/Components/ControllerComponent.h>
#include <Logic/Components/MouseInputListener.h>
#include <Logic/Components/Physics/Box2DPhysicsComponent.h>
#include <Logic/Components/TextComponent.h>
#include <Logic/Components/Animation/AnimationComponent.h>
#include <Logic/Components/RotationComponent.h>
#include <Logic/Components/Colliders/ColliderComponent.h>
#include <Logic/Components/Kinematic/KinematicComponent.h>
#include <Logic/Components/Particle/ParticleEmitterComponent.h>
#include <Logic/Collisions/CollisionSystem.h>
#include <Logic/Event/Events/CreateActorEvent.h>
#include <Logic/Event/Events/DestroyActorEvent.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Map/TiledMap.h>

#include <Utils/Random.h>

// Still need it here because of
#include <Lua/lua.hpp>

#include <assert.h>

using yang::IGameLayer;

bool yang::IGameLayer::Init(const yang::ApplicationLayer& app)
{
	LOG_CATEGORY(Lua, 0, Cyan, Light);

    m_pMap = std::make_shared<TiledMap>();
    m_pPhysics = app.GetPhysics();

    for (auto& pView : m_pViews)
    {
        if (!pView->Init(app))
        {
            return false;
        }
    }

    EventDispatcher::Get()->AddEventListener(DestroyActorEvent::kEventId, [this](IEvent* pEvent)
        {
            DestroyActor(static_cast<DestroyActorEvent*>(pEvent)->GetActorId());
        });

    EventDispatcher::Get()->AddEventListener(CreateActorEvent::kEventId, [this](IEvent* pEvent)
        {
            CreateActorEvent* pCreateActorEvent = static_cast<CreateActorEvent*>(pEvent);
            auto pFilepath = pCreateActorEvent->GetXmlFilepath();
            auto maybeLocation = pCreateActorEvent->GetSpawnPosition();
            SpawnActor(pFilepath, maybeLocation);
        });

    m_pCollisionSystem.reset(new CollisionSystem);
    RegisterComponents();

    m_pContactListener = new IContactListener(this);
    m_pPhysics->SetContactListener(m_pContactListener);

	if (!LuaState::GetInstance())
	{
		LOG(Error, "Failed to initialize Lua State");
		return false;
	}

	// Init scripting
	RegisterWithLua(LuaState::GetInstance()->GetRaw());

    return true;
}

void yang::IGameLayer::AddView(std::unique_ptr<IView> pView)
{
    pView->SetIndex(m_pViews.size());
    m_pViews.emplace_back(std::move(pView));
}

void yang::IGameLayer::Update(float deltaSeconds)
{
    for (auto& pActor : m_actorsToSpawn)
    {
        m_actors.emplace(pActor->GetId(), pActor);
    }
    m_actorsToSpawn.clear();

    for (auto& pView : m_pViews)
    {
        pView->UpdateInput();
    }

    // Consider that there might be a better place in the order of operations for this
    EventDispatcher::Get()->ProcessEvents();
    m_processManager.UpdateProcesses(deltaSeconds);

    for (auto& pActor : m_actors)
    {
        pActor.second->Update(deltaSeconds);
    }

    // When to update?
    m_pPhysics->Update(deltaSeconds);
    m_pCollisionSystem->Update(deltaSeconds);
    RespondToCollisions();

    for (auto& pView : m_pViews)
    {
        pView->ViewScene();
    }

    for (Id id : m_actorsToKill)
    {
        auto pActorIdPair = m_actors.find(id);
        if (pActorIdPair != m_actors.end())
        {
            auto pActorView = pActorIdPair->second->GetOwningView();

            if (pActorView)
            {
                pActorView->DetachActor();
                DeleteView(pActorView);
            }
            m_pCollisionSystem->ClearCollisionsWithActor(pActorIdPair->second.get());
            m_processManager.AbortProcessesOnActor(id);
            m_actors.erase(id);
        }
    }
    m_actorsToKill.clear();

    m_recentCollisions.clear();
}

void yang::IGameLayer::Cleanup()
{
    m_recentCollisions.clear();
    m_pPhysics->SetContactListener(nullptr);
    delete m_pContactListener;
    
    for (auto& pView : m_pViews)
    {
        pView->DetachActor();
        pView.reset();
    }

    m_actors.clear();

    m_pMap = nullptr;
}

void yang::IGameLayer::AddProcess(std::shared_ptr<IProcess> pProcess)
{
    m_processManager.AttachProcess(pProcess);
}

std::shared_ptr<yang::Actor> yang::IGameLayer::SpawnActor(const char* filepath, std::optional<FVec2> maybeLocation)
{
    auto pActor = m_actorFactory.CreateActor(filepath);

    if (pActor)
    {
        m_actorsToSpawn.emplace_back(pActor);
        if (auto pTransform = pActor->GetComponent<TransformComponent>(); pTransform && maybeLocation)
        {
            pTransform->SetPosition(*maybeLocation);
        }
    }

    return pActor;
}

void yang::IGameLayer::DestroyActor(Id actorId)
{
    m_actorsToKill.emplace_back(actorId);
}

void yang::IGameLayer::NotifyContact(Actor* pActorA, Actor* pActorB, bool isOverlap, ContactState state)
{
    m_recentCollisions.emplace_back(pActorA, pActorB, isOverlap, state);
}

void yang::IGameLayer::RegisterWithLua(lua_State* pState)
{
	// Let's create a global reference to our IGameLayer...
	lua_pushlightuserdata(pState, this);
	lua_setglobal(pState, "GameLayer");

	// I guess there is no other way to do that 
	lua_register(pState, "Log", Lua::Logger_Log);

	Actor::RegisterToLua(m_luaManager);
    AnimationComponent::RegisterToLua(m_luaManager);
    MouseInputListener::RegisterToLua(m_luaManager);
	TransformComponent::RegisterToLua(m_luaManager);
	SpriteComponent::RegisterToLua(m_luaManager);
	MoveComponent::RegisterToLua(m_luaManager);
	IGameLayer::RegisterToLua(m_luaManager);

}

void yang::IGameLayer::RegisterToLua(const LuaManager& manager)
{
	manager.ExposeToLua("GetActorById", &IGameLayer::GetActorById);
	manager.ExposeToLua("SpawnActor", &IGameLayer::SpawnActor);
	manager.ExposeToLua("DestroyActorById", &IGameLayer::DestroyActor);
}

void yang::IGameLayer::RegisterComponents()
{
    m_actorFactory.RegisterComponent<TransformComponent>();
    m_actorFactory.RegisterComponent<SpriteComponent>();
    m_actorFactory.RegisterComponent<MoveComponent>();
    m_actorFactory.RegisterComponent<ControllerComponent>();
    m_actorFactory.RegisterComponent<MouseInputListener>();
    m_actorFactory.RegisterComponent<Box2DPhysicsComponent>(m_pPhysics);
    m_actorFactory.RegisterComponent<TextComponent>();
    m_actorFactory.RegisterComponent<AnimationComponent>();
    m_actorFactory.RegisterComponent<RotationComponent>();
    m_actorFactory.RegisterComponent<ColliderComponent>(m_pCollisionSystem.get());
    m_actorFactory.RegisterComponent<KinematicComponent>();
    m_actorFactory.RegisterComponent<ParticleEmitterComponent>();
    //m_actorFactory.RegisterComponent<RectangleShape>();
}

void yang::IGameLayer::DeleteView(IView* pView)
{
    size_t index = pView->GetIndex();
    assert(m_pViews[index].get() == pView);
    DeleteView(index);
}

void yang::IGameLayer::RespondToCollisions()
{
    for (auto& [pActorA, pActorB, isOverlap, contactState] : m_recentCollisions)
    {
        if (isOverlap)
        {
            if (contactState == ContactState::kBegin)
            {
                pActorA->GetComponent<Box2DPhysicsComponent>()->HandleBeginOverlap(pActorB);
                pActorB->GetComponent<Box2DPhysicsComponent>()->HandleBeginOverlap(pActorA);
            }
            else
            {
                pActorA->GetComponent<Box2DPhysicsComponent>()->HandleEndOverlap(pActorB);
                pActorB->GetComponent<Box2DPhysicsComponent>()->HandleEndOverlap(pActorA);
            }
        }
        else
        {
            if (contactState == ContactState::kBegin)
            {
                pActorA->GetComponent<Box2DPhysicsComponent>()->HandleBeginCollision(pActorB);
                pActorB->GetComponent<Box2DPhysicsComponent>()->HandleBeginCollision(pActorA);
            }
            else
            {
                pActorA->GetComponent<Box2DPhysicsComponent>()->HandleEndCollision(pActorB);
                pActorB->GetComponent<Box2DPhysicsComponent>()->HandleEndCollision(pActorA);
            }
        }
    }

    m_recentCollisions.clear();
}

void yang::IGameLayer::DeleteView(size_t index)
{
    assert(index < m_pViews.size());
    std::swap(m_pViews[index], m_pViews[m_pViews.size() - 1]);
    m_pViews[index]->SetIndex(index);
    m_pViews.pop_back();
    
}

yang::Actor* yang::IGameLayer::GetActorById(Id id) const
{
	return m_actors.count(id) ? m_actors.at(id).get() : nullptr;
}