#pragma once
/** \file DestroyActorEvent.h */
/** Destroy Actor Event description */

#include "..\IEvent.h"
#include <Utils/Typedefs.h>

//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class DestroyActorEvent */
/** Event for actor destruction */
class DestroyActorEvent
	: public IEvent
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    // {8F5A1428-E1EA-4E0A-AA21-61252B2006C7}
    /// Event ID
    static constexpr EventId kEventId = 0x8f5a1428;

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

    /// Constructor
    /// \param actorId - ID of the actor to destroy
	DestroyActorEvent(Id actorId);

	/** Default Destructor */
	~DestroyActorEvent();

    /// Get the Event ID
    EventId GetEventId() const override { return kEventId; }

    /// Get the name of the event
    /// \return "DestroyActor"
    const char* GetName() const override { return "DestroyActor"; }

private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    Id m_actorId;   ///< Id of the actor to destroy

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //


public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //

    /// Get the destroying actor id
    Id GetActorId() const { return m_actorId; }

};
}