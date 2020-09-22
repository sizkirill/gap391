#include "MouseMotionEvent.h"

using yang::MouseMotionEvent;

MouseMotionEvent::MouseMotionEvent(const yang::IVec2& from, const yang::IVec2& to)
    :m_from(from)
    ,m_to(to)
{
	
}

MouseMotionEvent::~MouseMotionEvent()
{
	
}