#pragma once
/** \file MouseMotionEvent.h */
/** Mouse Motion Event description */

#include "..\IEvent.h"
#include <Utils/Math.h>
#include <Utils/Vector2.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class MouseMotionEvent */
/** Mouse Input Event that is responsible for mouse motion */
class MouseMotionEvent
	: public IEvent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    // {389AB423-7761-4DCC-9527-D93ECED0A038}
    /// Event ID
    static constexpr EventId kEventId = 0x389ab423;


	/// Constructor
    /// \param from - position on the screen the mouse cursor started moving
    /// \param to - position on the screen the mouse cursor ended moving
	MouseMotionEvent(const IVec2& from, const IVec2& to);

	/** Default Destructor */
	~MouseMotionEvent();


private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    IVec2 m_from;      ///< position on the screen the mouse cursor started moving
    IVec2 m_to;        ///< position on the screen the mouse cursor ended moving

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
    const char* GetName() const override { return "MouseMotion"; }
    /// Accessor for starting point
    const IVec2& GetStartPoint() const { return m_from; }
    /// Accessor for end point
    const IVec2& GetEndPoint() const { return m_to; }

};
}