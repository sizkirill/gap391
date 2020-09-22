#pragma once
#include <Utils/Typedefs.h>
/** \file IEvent.h */
/** Base Event interface description */


//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class IEvent */
/** Base class for all events */
class IEvent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    /// Alias for event Ids
    using EventId = Id;

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Constructor */
	IEvent();

	/** Default Destructor */
	virtual ~IEvent();

    /// Get the event ID
    virtual EventId GetEventId() const = 0;

    /// Get the name of the event
    virtual const char* GetName() const = 0;
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //


	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}