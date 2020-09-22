#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

#include <Logic/Event/IEvent.h>
/** \file EventDispatcher.h */
/** EventDispatcher class description */


//! \namespace yang Contains all Yangine code
namespace yang
{
/** \class EventDispatcher */
/** Registers and dispatches events to listeners. Singleton class */
class EventDispatcher
{
public:
	// --------------------------------------------------------------------- //
	// Public Member Variables
	// --------------------------------------------------------------------- //

    /// Alias for event IDs
    using EventId = IEvent::EventId;

	// --------------------------------------------------------------------- //
	// Public Member Functions
	// --------------------------------------------------------------------- //

	/** Default Destructor */
	~EventDispatcher();

    /// Get EventDispatcher singleton instance
    static EventDispatcher* Get();

    /// Adds event listener for an event
    /// \param id - Event id to listen
    /// \param listener - Callback to execute when event triggers
    /// \return - index of the listener
    size_t AddEventListener(EventId id, const std::function<void(IEvent*)>& listener);

    /// Removes event listener
    /// \param id - Event id, from which to remove listener
    /// \param index - Index of the listener to remove
    void RemoveEventListener(EventId id, size_t index);

    /// Queues event to be triggered next time ProcessEvents() will be called
    /// \param pEvent - unique ptr to an event to trigger
    void QueueEvent(std::unique_ptr<IEvent> pEvent);

    /// Triggers event immediately. Not responsible for cleaning up the event (takes in a raw pointer)
    /// \param pEvent - pointer to an event to trigger
    void TriggerEvent(IEvent* pEvent);

    /// Triggers event immediately.
    /// \param pEvent - unique ptr to an event to trigger
    void TriggerEvent(std::unique_ptr<IEvent> pEvent);

    /// Processes all queued events. Not intended to be called manually outside of the game main loop.
    void ProcessEvents();
private:
	// --------------------------------------------------------------------- //
	// Private Member Variables
	// --------------------------------------------------------------------- //
    
    /// Map of vectors of callbacks. When an event is triggered, the vector will be looped through and all callbacks for an event will be called
    std::unordered_map<EventId, std::vector<std::function<void(IEvent*)>>> m_eventListeners;

    /// Queue of events that will be handled on the next frame
    std::vector<std::unique_ptr<IEvent>> m_queue;

	// --------------------------------------------------------------------- //
	// Private Member Functions
	// --------------------------------------------------------------------- //
    /** Default Constructor */
    EventDispatcher();

public:
	// --------------------------------------------------------------------- //
	// Accessors & Mutators
	// --------------------------------------------------------------------- //


};
}