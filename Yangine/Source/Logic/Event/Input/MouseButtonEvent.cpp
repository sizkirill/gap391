#include "MouseButtonEvent.h"

using yang::MouseButtonEvent;

MouseButtonEvent::MouseButtonEvent(MouseButton button, EventType eventType, const yang::IVec2& position)
    :m_mouseButton(button)
    ,m_eventType(eventType)
    ,m_clickPosition(position)
{
	
}

MouseButtonEvent::~MouseButtonEvent()
{
	
}