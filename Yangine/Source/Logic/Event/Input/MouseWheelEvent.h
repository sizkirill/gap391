#pragma once
/** \file MouseWheelEvent.h */
/** Mouse Wheel Event description */

#include "..\IEvent.h"
#include <Utils/Math.h>
#include <Utils/Vector2.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class MouseWheelEvent */
/** Mouse Input Event that is responsible for mouse wheel scrolling */
class MouseWheelEvent
	: public IEvent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    // {6C312169-760B-4598-9C08-6522035397EF}
    /// Event ID
    static constexpr EventId kEventId = 0x6c312169;

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param scrollAmount - IVec2 representing the amount of scroll
	MouseWheelEvent(const IVec2& scrollAmount);

	/** Default Destructor */
	~MouseWheelEvent();


private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    IVec2 m_scrollAmount;   ///< Scroll amount

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
    const char* GetName() const override { return "MouseWheel"; }
    /// Accessor for scroll amount
    const IVec2& GetScrollAmount() const { return m_scrollAmount; }

};
}