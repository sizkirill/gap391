#pragma once
/** \file KeyboardInputEvent.h */
/**Keyboard Input Event description */

#include "..\IEvent.h"
#include <Application/Input/IKeyboard.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class KeyboardInputEvent */
/** Event for processing keyboard input */
class KeyboardInputEvent
	: public IEvent
{
public:

    /// Alias for the key code of the button the event associated with
    using KeyCode = IKeyboard::KeyCode;
    
    /// \enum EventType
    /// Type of the event
    enum class EventType
    {
        kKeyPressed,    ///< Button was just pressed
        kKeyReleased,   ///< Button was just released
        kKeyDown,       ///< Button is down
        kMaxTypes       ///< Max count of supported event types
    };
    // --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    // {DBC84AC4-95A5-4A43-8C9A-CF72F240AE21}
    /// Event ID
    static constexpr EventId kEventId = 0xdbc84ac4;


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param keyCode - KeyCode of the button that event is associated with. \see yang::IKeyboard::KeyCode
    /// \param eventType - EventType of the keyboard event. \see yang::KeyboardInputEvent::EventType
	KeyboardInputEvent(KeyCode keyCode, EventType eventType);

	/** Default Destructor */
	~KeyboardInputEvent();

    /// Get Event ID
    EventId GetEventId() const override final { return kEventId; }

    /// Get the name of the event
    /// \return "KeyboardInput"
    const char* GetName() const override final { return "KeyboardInput"; }
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    KeyCode m_keyCode;          ///< KeyCode this event is associated with. \see yang::IKeyboard::KeyCode
    EventType m_eventType;      ///< Keyboard event type this event is associated with. \see yang::KeyboardInputEvent::EventType

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Get the KeyCode this event is associated with. \see yang::IKeyboard::KeyCode
    KeyCode GetKeyCode() const { return m_keyCode; }
    /// Get EventType this event is associated with. \see yang::KeyboardInputEvent::EventType
    EventType GetEventType() const { return m_eventType; }

};
}