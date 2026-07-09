#include "events.h"
#include <map>
#include <vector>

static std::map<EventType, std::vector<EventDispatcher::Callback>> listeners;

void EventDispatcher::subscribe(EventType type, Callback callback)
{
    listeners[type].push_back(callback);
}

void EventDispatcher::sendEvent(const Event& event)
{
    if(listeners.find(event.type) != listeners.end()) {
        for (auto callback : listeners[event.type]) {
            callback(event);
        }
    }
}

