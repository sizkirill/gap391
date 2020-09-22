#include "IMouse.h"
#include <Utils/Logger.h>
#include <Logic/Event/EventDispatcher.h>
#include <Logic/Event/Input/MouseMotionEvent.h>
#include <Logic/Event/Input/MouseWheelEvent.h>
#include <Logic/Event/Input/MouseButtonEvent.h>
#include <cstring>

#include <assert.h>

using yang::IMouse;

yang::IMouse::IMouse()
{
    memset(m_buttonState, 0, sizeof(m_buttonState));
    memset(m_previousButtonState, 0, sizeof(m_previousButtonState));
}

bool yang::IMouse::Init()
{
    return true;
}

void yang::IMouse::SetButtonState(MouseButton button, bool isPressed)
{
    using EventType = MouseButtonEvent::EventType;
    if (button != MouseButton::kMaxButtons)
    {
        EventDispatcher* pDispatcher = EventDispatcher::Get();
        assert(pDispatcher);

        m_buttonState[static_cast<size_t>(button)] = isPressed;

        pDispatcher->TriggerEvent(std::make_unique<MouseButtonEvent>(button, isPressed ? EventType::kButtonPressed : EventType::kButtonReleased, m_position));
    }
}

bool yang::IMouse::IsButtonDown(MouseButton button)
{
    return m_buttonState[static_cast<size_t>(button)];
}

bool yang::IMouse::IsButtonPressed(MouseButton button)
{
    return m_buttonState[static_cast<size_t>(button)] && !m_previousButtonState[static_cast<size_t>(button)];
}

bool yang::IMouse::IsButtonReleased(MouseButton button)
{
    return m_buttonState[static_cast<size_t>(button)] && !m_previousButtonState[static_cast<size_t>(button)];
}

void yang::IMouse::NextFrame()
{
    assert(sizeof(m_buttonState) == sizeof(m_previousButtonState));
    memcpy(m_previousButtonState, m_buttonState, sizeof(m_buttonState));
}

void yang::IMouse::ProcessWheelEvent(bool isNormal, i32 horizontalAmount, i32 verticalAmount)
{
    if (!isNormal)
    {
        horizontalAmount = -horizontalAmount;
        verticalAmount = -verticalAmount;
    }

    EventDispatcher* pDispatcher = EventDispatcher::Get();
    assert(pDispatcher);

    pDispatcher->TriggerEvent(std::make_unique<MouseWheelEvent>(IVec2(horizontalAmount, verticalAmount)));
}

std::unique_ptr<IMouse> yang::IMouse::Create()
{
    return std::make_unique<IMouse>();
}

void yang::IMouse::SetMousePosition(int x, int y)
{
    EventDispatcher* pDispatcher = EventDispatcher::Get();
    assert(pDispatcher);

    pDispatcher->TriggerEvent(std::make_unique<MouseMotionEvent>(m_position, IVec2(x,y)));

    m_position = IVec2(x,y);
}
