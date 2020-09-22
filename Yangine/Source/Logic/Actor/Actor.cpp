#include "Actor.h"
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Events/DestroyActorEvent.h>
#include <Utils/Logger.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/StringHash.h>

#include <Logic/Scripting/LuaManager.h>
#include <Lua/lua.hpp>

using yang::Actor;

yang::Actor::Actor(Id id)
    :m_id(id)
    , m_pOwningView(nullptr)
{
}

Actor::~Actor()
{
    m_components.clear();
}

bool yang::Actor::Init(tinyxml2::XMLElement* pData)
{
    std::string name(pData->Name());
    if (name != "Actor")
    {
        LOG(Error, "XML Node is not an actor, it is %s", pData->Name());
        return false;
    }

    if (const char* pTag = pData->Attribute("tag"); pTag != nullptr)
    {
        m_tag = pTag;
    }
    else
    {
        m_tag = "Unknown";
    }
    m_hashTag = StringHash32(m_tag.c_str());

    return true;
}

bool yang::Actor::PostInit()
{
    for (auto& componentPair : m_components)
    {
        if (!componentPair.second->PostInit())
        {
            LOG(Error, "Actor failed to PostInit. Actor ID: %d", GetId());
            return false;
        }
    }
    return true;
}

void yang::Actor::Destroy()
{
    EventDispatcher::Get()->QueueEvent(std::make_unique<DestroyActorEvent>(m_id));
}

void yang::Actor::Update(float deltaSeconds)
{
    for (auto& componentPair : m_components)
    {
        componentPair.second->Update(deltaSeconds);
    }
}

void yang::Actor::Render(IGraphics* pGraphics)
{
    LOG_ONCE(TODO, ConstCorrectness, "Function probably should be const");
    for (auto& componentPair : m_components)
    {
        componentPair.second->Render(pGraphics);
    }
}

void yang::Actor::AddComponent(std::unique_ptr<IComponent> pComponent)
{
    if (!pComponent)
    {
        LOG(Error, "Cannot add a null component to actor (ID: %d)", m_id);
        return;
    }

    // Consider change to smth like reset?
    m_components[pComponent->GetId()] = std::move(pComponent);
}

yang::IComponent* yang::Actor::GetComponent(Id id) const
{
    auto itr = m_components.find(id);
    if (itr == m_components.end())
    {
        LOG(Warning, "Actor (ID: %d) doesn't have that component (ID: %d)", m_id, id);
        return nullptr;
    }
    return itr->second.get();
}

yang::IComponent* yang::Actor::GetComponent(const char* name) const
{
	return GetComponent(yang::IComponent::HashName(name));
}

void yang::Actor::RegisterToLua(const LuaManager& manager)
{
	manager.ExposeToLua<IComponent*(Actor::*)(const char*) const>("GetComponent", &Actor::GetComponent);
}