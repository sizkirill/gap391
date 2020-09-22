#include "ControllerComponent.h"
#include <Application/Input/IKeyboard.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Actor/Actor.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/Logger.h>
#include <Utils/Typedefs.h>

#include <Logic/Components/MoveComponent.h>
#include <Logic/Components/Physics/Box2DPhysicsComponent.h>

#include <Utils/StringHash.h>
using yang::ControllerComponent;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> IComponent::CreateComponent<StringHash32(ControllerComponent::GetName())>(yang::Actor* pOwner)
{
    return std::make_unique<ControllerComponent>(pOwner);
};

#pragma warning(pop)

ControllerComponent::ControllerComponent(yang::Actor* pOwner)
    :IComponent(pOwner, GetName())
    ,m_eventListenerId(kInvalidValue<size_t>)
{
	
}

bool yang::ControllerComponent::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    XMLElement* pKeyboardData = pData->FirstChildElement("Keyboard");

    // I guess this is gonna change anyway after integrating Lua, so whatever for now.
    for (XMLElement* pElement = pKeyboardData->FirstChildElement(); pElement != nullptr; pElement = pElement->NextSiblingElement())
    {
        const char* pKeycodeName = pElement->Attribute("keyCode");
        if (!pKeycodeName)
        {
            LOG(Error, "keyCode should be present");
            return false;
        }

        const char* pComponentName = pElement->Attribute("targetComponent");
        if (pComponentName)
        {
            LOG_ONCE(TODO, Controller, "Revisit after integrating LUA");

            Id targetComponentId = IComponent::HashName(pComponentName);

            XMLElement* pOnKeyDown = pElement->FirstChildElement("OnKeyDown");
            XMLElement* pOnKeyPressed = pElement->FirstChildElement("OnKeyPressed");
            XMLElement* pOnKeyReleased = pElement->FirstChildElement("OnKeyReleased");

            if (targetComponentId == IComponent::HashName(MoveComponent::GetName()))
            {
                IKeyboard::KeyCode keyCode = IKeyboard::KeyCodeFromString(pKeycodeName);
                if (pOnKeyDown)
                {
                    XMLElement* pDirection = pOnKeyDown->FirstChildElement("Direction");
                    if (pDirection)
                    {
                        float x = pDirection->FloatAttribute("x", kInvalidFloat);
                        float y = pDirection->FloatAttribute("y", kInvalidFloat);
                        m_keyActionMap[keyCode].emplace_back(KeyboardInputEvent::EventType::kKeyDown, [this, x, y]()
                            {
                                MoveComponent* pMoveComp = GetOwner()->GetComponent<MoveComponent>();
                                if (pMoveComp)
                                {
                                    auto direction = pMoveComp->GetVelocity();
                                    if (x != kInvalidFloat)
                                        pMoveComp->SetVelocity(FVec2(x, direction.y));
                                    if (y != kInvalidFloat)
                                        pMoveComp->SetVelocity(FVec2(direction.x, y));
                                }
                            });
                    }
                    else
                    {
                        LOG(Error, "No direction found");
                    }
                }

                if (pOnKeyPressed)
                {
                    XMLElement* pDirection = pOnKeyPressed->FirstChildElement("Direction");
                    if (pDirection)
                    {
                        float x = pDirection->FloatAttribute("x", kInvalidFloat);
                        float y = pDirection->FloatAttribute("y", kInvalidFloat);
                        m_keyActionMap[keyCode].emplace_back(KeyboardInputEvent::EventType::kKeyPressed, [this, x, y]()
                            {
                                MoveComponent* pMoveComp = GetOwner()->GetComponent<MoveComponent>();
                                if (pMoveComp)
                                {
                                    auto direction = pMoveComp->GetVelocity();
                                    if (x != kInvalidFloat)
                                        pMoveComp->SetVelocity(FVec2(x, direction.y));
                                    if (y != kInvalidFloat)
                                        pMoveComp->SetVelocity(FVec2(direction.x, y));
                                }
                            });
                    }
                    else
                    {
                        LOG(Error, "No direction found");
                    }
                }

                if (pOnKeyReleased)
                {
                    XMLElement* pDirection = pOnKeyReleased->FirstChildElement("Direction");
                    if (pDirection)
                    {
                        float x = pDirection->FloatAttribute("x", kInvalidFloat);
                        float y = pDirection->FloatAttribute("y", kInvalidFloat);
                        m_keyActionMap[keyCode].emplace_back(KeyboardInputEvent::EventType::kKeyReleased, [this, x, y]()
                            {
                                MoveComponent* pMoveComp = GetOwner()->GetComponent<MoveComponent>();
                                if (pMoveComp)
                                {
                                    auto direction = pMoveComp->GetVelocity();
                                    if (x != kInvalidFloat)
                                        pMoveComp->SetVelocity(FVec2(x, direction.y));
                                    if (y != kInvalidFloat)
                                        pMoveComp->SetVelocity(FVec2(direction.x, y));
                                }
                            });
                    }
                    else
                    {
                        LOG(Error, "No direction found");
                    }
                }
            }
        }
        else
        {
            LOG(Error, "targetComponent should be present");
            return false;
        }
    }

    return true;
}

bool yang::ControllerComponent::PostInit()
{
    for (auto id : m_targetIds)
    {
        if (!GetOwner()->GetComponent(id))
        {
            LOG(Error, "Component with ID %d is not present", id);
            return false;
        }
    }

    m_eventListenerId = EventDispatcher::Get()->AddEventListener(KeyboardInputEvent::kEventId, [this](IEvent* pEvent) { HandleKeyInputEvent(pEvent); });

    return true;
}

ControllerComponent::~ControllerComponent()
{
    EventDispatcher::Get()->RemoveEventListener(KeyboardInputEvent::kEventId, m_eventListenerId);
}

void yang::ControllerComponent::HandleKeyInputEvent(IEvent* pEvent)
{
    if (pEvent->GetEventId() == KeyboardInputEvent::kEventId)
    {
        KeyboardInputEvent* pKeyboardEvent = static_cast<KeyboardInputEvent*>(pEvent);

        KeyboardInputEvent::EventType eventType = pKeyboardEvent->GetEventType();
        IKeyboard::KeyCode keyCode = pKeyboardEvent->GetKeyCode();

        if (m_keyActionMap.count(keyCode))
        {
            for (auto& pair : m_keyActionMap[keyCode])
            {
                if (pair.first == eventType)
                {
                    pair.second();
                }
            }
        }
    }
}

