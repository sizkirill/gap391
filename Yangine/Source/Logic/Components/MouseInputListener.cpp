#include "MouseInputListener.h"
#include <Logic/Actor/Actor.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Events/CreateActorEvent.h>
#include <Logic/Event/Input/MouseMotionEvent.h>
#include <Logic/Event/Input/MouseButtonEvent.h>
#include <Logic/Event/Input/MouseWheelEvent.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Scripting/LuaManager.h>
#include <Logic/Scripting/LuaState.h>
#include <Utils/Rectangle.h>
#include <Utils/Logger.h>
#include <Utils/Typedefs.h>
#include <Utils/TinyXml2/tinyxml2.h>
#include <Utils/StringHash.h>
#include <cassert>

#define ATTACH_FOR_TEST(ElementName)\
    {\
    std::string name = "O";\
    name += #ElementName;\
    XMLElement* pO##ElementName = pData->FirstChildElement(name.c_str());\
    if (pO##ElementName)\
    {\
        m_o##ElementName = []() {};\
    }\
    }

using yang::MouseInputListener;
using yang::IEvent;
using yang::Actor;
using yang::IComponent;

#pragma warning(push)
#pragma warning(disable:4307)

template<>
std::unique_ptr<IComponent> IComponent::CreateComponent<StringHash32(MouseInputListener::GetName())>(Actor* pOwner)
{
    return std::make_unique<MouseInputListener>(pOwner);
};

#pragma warning(pop)

MouseInputListener::MouseInputListener(Actor* pOwner)
    :IComponent(pOwner, GetName())
    ,m_onClick(nullptr)
    , m_onMouseOver(nullptr)
    , m_onMouseOut(nullptr)
    ,m_onDoubleClick(nullptr)
    ,m_onScrollWheel(nullptr)
    ,m_mouseMotionListenerIndex(kInvalidValue<size_t>)
    ,m_clickListenerIndex(kInvalidValue<size_t>)
    ,m_wheelListenerIndex(kInvalidValue<size_t>)
{
	
}

MouseInputListener::~MouseInputListener()
{
    EventDispatcher* pDispatcher = EventDispatcher::Get();
    assert(pDispatcher);

    if (IsValid(m_mouseMotionListenerIndex))
        pDispatcher->RemoveEventListener(MouseMotionEvent::kEventId, m_mouseMotionListenerIndex);

    if (IsValid(m_clickListenerIndex))
        pDispatcher->RemoveEventListener(MouseButtonEvent::kEventId, m_clickListenerIndex);
}

void yang::MouseInputListener::HandleMouseMotion(IEvent* pEvent)
{
    assert(MouseMotionEvent::kEventId == pEvent->GetEventId());
    MouseMotionEvent* pResult = static_cast<MouseMotionEvent*>(pEvent);

    IRect collider(m_pTransform->GetPosition(), m_colliderDimensions);
    if (!collider.Contains(pResult->GetStartPoint()) &&
        collider.Contains(pResult->GetEndPoint()))
    {
        LOG(Info, "OnMouseOver triggered at position: x = %d, y = %d", pResult->GetEndPoint().x, pResult->GetEndPoint().y);
        if (m_onMouseOver)
        {
            m_onMouseOver();
        }

        m_onMouseOverLua.Call<void>();
    }
    else if (collider.Contains(pResult->GetStartPoint()) &&
             !collider.Contains(pResult->GetEndPoint()))
    {
        LOG(Info, "OnMouseOut triggered at position: x = %d, y = %d", pResult->GetEndPoint().x, pResult->GetEndPoint().y);
        if (m_onMouseOut)
        {
            m_onMouseOut();
        }
        m_onMouseOutLua.Call<void>();
    }
}

void yang::MouseInputListener::HandleClick(IEvent* pEvent)
{
    assert(MouseButtonEvent::kEventId == pEvent->GetEventId());
    MouseButtonEvent* pResult = static_cast<MouseButtonEvent*>(pEvent);

    IRect collider(m_pTransform->GetPosition(), m_colliderDimensions);
    if (pResult->GetEventType() == MouseButtonEvent::EventType::kButtonPressed &&
        pResult->GetMouseButton() == IMouse::MouseButton::kLeft)
    {
        LOG(Info, "OnClick triggered at position: x = %d, y = %d", pResult->GetClickPosition().x, pResult->GetClickPosition().y);
        if (m_onClick)
        {
            m_onClick(pResult->GetClickPosition());
        }
        m_onClickLua.Call<void>(pResult->GetClickPosition());
    }
    else if (pResult->GetEventType() == MouseButtonEvent::EventType::kButtonPressed &&
             pResult->GetMouseButton() == IMouse::MouseButton::kRight)
    {
        LOG(Info, "OnRightClick triggered at position: x = %d, y = %d", pResult->GetClickPosition().x, pResult->GetClickPosition().y);
        if (m_onRightClick)
        {
            m_onRightClick(pResult->GetClickPosition());
        }
        m_onRightClickLua.Call<void>(pResult->GetClickPosition());
    }

    LOG_ONCE(TODO, ImplementDoubleClick, "");
}

void yang::MouseInputListener::HandleWheelScroll(IEvent* pEvent)
{
    assert(MouseWheelEvent::kEventId == pEvent->GetEventId());
    MouseWheelEvent* pResult = static_cast<MouseWheelEvent*>(pEvent);
    if (m_onScrollWheel)
    {
        LOG(Info, "OnScrollWheel triggered with amount: x = %d, y = %d", pResult->GetScrollAmount().x, pResult->GetScrollAmount().y);

        // So this can crash probably
        m_onScrollWheel(pResult->GetScrollAmount());
    }
    m_onScrollWheelLua.Call<void>(pResult->GetScrollAmount());
}

void yang::MouseInputListener::RegisterToLua(const LuaManager& manager)
{
    manager.ExposeToLua("SetOnClick", &MouseInputListener::SetOnClickLua);
    manager.ExposeToLua("SetOnRightClick", &MouseInputListener::SetOnRightClickLua);
    manager.ExposeToLua("SetOnMouseOver", &MouseInputListener::SetOnMouseOverLua);
    manager.ExposeToLua("SetOnMouseOut", &MouseInputListener::SetOnMouseOutLua);
    manager.ExposeToLua("SetOnScrollWheel", &MouseInputListener::SetOnScrollWheelLua);
}

bool yang::MouseInputListener::Init(tinyxml2::XMLElement* pData)
{
    using namespace tinyxml2;

    XMLElement* pDimensions = pData->FirstChildElement("Dimensions");
    if (pDimensions)
    {
        m_colliderDimensions.x = pDimensions->IntAttribute("width");
        m_colliderDimensions.y = pDimensions->IntAttribute("height");
    }
    else
    {
        LOG(Warning, "No dimensions for mouse input listener collider specified. Using default");
    }

    // Some gross macros for tests until there will be actually working parsing for this (lua maybe?)

    ATTACH_FOR_TEST(nMouseOver);
    ATTACH_FOR_TEST(nMouseOut);
    ATTACH_FOR_TEST(nDoubleClick);

    XMLElement* pWheelElement = pData->FirstChildElement("OnScrollWheel");
    if (pWheelElement)
    {
        m_onScrollWheel = [](const IVec2& scrollAmount) {};
    }

    return true;
}

bool yang::MouseInputListener::PostInit()
{
    LOG_ONCE(TODO, MouseInputListenerPostInit, "Change this function, so it registers itself to events event if it doesn't have attached functions. For lua purposes");

    m_pTransform = GetOwner()->GetComponent<TransformComponent>();
    if (!m_pTransform)
    {
        LOG(Error, "Mouse input listener doesn't have any sense without transform attached");
        return false;
    }

    EventDispatcher* pDispatcher = EventDispatcher::Get();
    assert(pDispatcher);

    bool hasMouseMotion = m_onMouseOver || m_onMouseOut;
    bool hasClick = m_onClick || m_onRightClick || m_onDoubleClick;
    bool hasWheel = m_onScrollWheel != nullptr;
    bool hasNothing = !hasMouseMotion && !hasClick && !hasWheel;

    if (hasMouseMotion)
    {
        m_mouseMotionListenerIndex = pDispatcher->AddEventListener(MouseMotionEvent::kEventId, [this](IEvent* pEvent) {HandleMouseMotion(pEvent); });
    }
    
    if (hasClick)
    {
        m_clickListenerIndex = pDispatcher->AddEventListener(MouseButtonEvent::kEventId, [this](IEvent* pEvent) {HandleClick(pEvent); });
    }

    if (hasWheel)
    {
        m_wheelListenerIndex = pDispatcher->AddEventListener(MouseWheelEvent::kEventId, [this](IEvent* pEvent) {HandleWheelScroll(pEvent); });
    }
    
    if (hasNothing)
    {
        LOG(Warning, "Listener is deaf, why is it here?");
    }

    return true;
}
