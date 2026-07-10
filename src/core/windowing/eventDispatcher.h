#pragma once
#include "events.h"

namespace Lumos
{
    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            :m_event(event)
        {
        }

        template<typename T, typename F>
        bool dispatch(const F& func)
        {
            if (m_event.eventType() == T::staticType()) {
                m_event.handled |= func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }
    private:
        Event& m_event;
    };
}//namespace Lumos
