#pragma once
/** \file ControllerComponent.h */
/** ControllerComponent description */

#include ".\IComponent.h"
#include <Application/Input/IKeyboard.h>
#include <Logic/Event/Input/KeyboardInputEvent.h>

#include <unordered_map>
#include <functional>

//! \namespace yang Contains all Yangine code
namespace yang
{
    class EventDispatcher;
/** \class ControllerComponent */
/** Actor component that handles keyboard input to control the actor */
class ControllerComponent
	: public IComponent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/// Constructor
    /// \param pOwner - actor that owns this component
	ControllerComponent(Actor* pOwner);

    /// Name of this component, used for hashing
    /// \return "ControllerComponent"
    static constexpr const char* GetName() { return "ControllerComponent"; }

    /// Initialize the component from an XML element
    /// \param pData - XML Element that contains component data
    /// \return true if initialized successfully
    virtual bool Init(tinyxml2::XMLElement* pData) override final;

    /// Obtains IDs of needed components
    /// \return true if components were successfully found
    virtual bool PostInit() override final;

    /// Callback for event system, that handles the keyboard event
    /// \param pEvent - event to handle
    void HandleKeyInputEvent(IEvent* pEvent);

	/** Default Destructor */
	~ControllerComponent();


private:

    /// Alias for pair of Keyboard event type and associated callback function. \see yang::KeyboardInputEvent::EventType
    using Action = std::pair<KeyboardInputEvent::EventType, std::function<void()>>;

    /// Alias for the lookup table of callbacks. \see yang::ControllerComponent::Action
    using KeyActionMap = std::unordered_map<IKeyboard::KeyCode, std::vector<Action>>;

	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    KeyActionMap m_keyActionMap;    ///< Lookup table for callbacks
    std::vector<Id> m_targetIds;    ///< Associated components ID's
    size_t m_eventListenerId;       ///< Index of the event listener, associated with the keyboard input event

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

};
}