#pragma once
/** \file MouseButtonEvent.h */
/** Mouse Button Event description */

#include "..\IEvent.h"
#include <Utils/Math.h>
#include <Application/Input/IMouse.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class MouseButtonEvent */
/** Mouse Input Event that is responsible for mouse buttons */
class MouseButtonEvent
	: public IEvent
{
public:
    /// Alias for IMouse::MouseButton
    using MouseButton = IMouse::MouseButton;
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    /// \enum EventType
    /// Describes the type of mouse button event
    enum class EventType
    { 
        kButtonPressed,     ///< Button was pressed
        kButtonReleased,    ///< Button was released
        kMaxTypes           ///< Invalid event
    };

    // {0E905C9E-D34A-47DA-B265-F10D8F6DAA2F}
    /// Event ID
    static constexpr EventId kEventId = 0xe905c9e;

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param button - Button that the event is associated to
    /// \param eventType - Type of a mouse event \sa enum EventType
    /// \param position - IVec2, representing position of the mouse
	MouseButtonEvent(MouseButton button, EventType eventType, const IVec2& position);

	/** Default Destructor */
	~MouseButtonEvent();


private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    EventType m_eventType;      ///< Mouse event type this event is associated with. \see yang::MouseInputEvent::EventType 
    MouseButton m_mouseButton;  ///< MouseButton this event is associated with. \see yang::IMouse::KeyCode
    IVec2 m_clickPosition;      ///< Position on the screen of the click happened

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Accessor for event id
    EventId GetEventId() const override { return kEventId; }
    /// Accessor for event name
    const char* GetName() const override { return "MouseButton"; }
    /// Accessor for mouse button \see yang::IMouse::MouseButton
    MouseButton GetMouseButton() const { return m_mouseButton; }
    /// Accessor for event type \see yang::MouseButtonEvent::EventType
    EventType GetEventType() const { return m_eventType; }
    /// Accessor for click position
    IVec2 GetClickPosition() const { return m_clickPosition; }

};
}