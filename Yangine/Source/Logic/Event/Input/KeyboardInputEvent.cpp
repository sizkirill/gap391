#include "KeyboardInputEvent.h"

using yang::KeyboardInputEvent;

yang::KeyboardInputEvent::KeyboardInputEvent(KeyCode keyCode, EventType eventType)
    :m_keyCode(keyCode)
    ,m_eventType(eventType)
{
}

KeyboardInputEvent::~KeyboardInputEvent()
{
	
}