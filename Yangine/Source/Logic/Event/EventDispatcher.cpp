#include "EventDispatcher.h"
#include <Utils/Logger.h>

using yang::EventDispatcher;

EventDispatcher::EventDispatcher()
{
	
}

EventDispatcher::~EventDispatcher()
{
	
}

/* static */ EventDispatcher* yang::EventDispatcher::Get()
{
    static EventDispatcher s_instance;
    return &s_instance;
}

size_t yang::EventDispatcher::AddEventListener(EventId id, const std::function<void(IEvent*)>& listener)
{
    // Grab the array of listeners associated with this event
    auto& listeners = m_eventListeners[id];

    // Loop through the vector looking for an open slot
    for (size_t index = 0; index < listeners.size(); ++index)
    {
        if (!listeners[index])
        {
            listeners[index] = listener;
            return index;
        }
    }

    // No open slots
    listeners.emplace_back(listener);
    return listeners.size() - 1;
}

void yang::EventDispatcher::RemoveEventListener(EventId id, size_t index)
{
    auto& listeners = m_eventListeners[id];
    if (index < listeners.size())
    {
        listeners[index] = nullptr;
    }
    else
    {
        LOG(Error, "Invalid index received when attempting to remove event listener from dispatcher");
    }
}

void yang::EventDispatcher::QueueEvent(std::unique_ptr<IEvent> pEvent)
{
    m_queue.push_back(std::move(pEvent));
}

void yang::EventDispatcher::TriggerEvent(IEvent* pEvent)
{
    auto& listeners = m_eventListeners[pEvent->GetEventId()];
    for (auto& listener : listeners)
    {
        if (listener)
        {
            listener(pEvent);
        }
    }
}

void yang::EventDispatcher::TriggerEvent(std::unique_ptr<IEvent> pEvent)
{
    TriggerEvent(pEvent.get());
}

void yang::EventDispatcher::ProcessEvents()
{
    // Move our queue over to a local variable. This means that any events added throughout the course of this loop
    // will be handled next frame
    auto processingQueue = std::move(m_queue);
    for (auto& pEvent : processingQueue)
    {
        TriggerEvent(pEvent.get());
    }
}
