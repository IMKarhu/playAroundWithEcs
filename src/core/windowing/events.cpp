#include "events.h"

namespace Lumos
{
    /*MOUSE RELATED EVENTS*/
    MouseButtonPressedEvent::MouseButtonPressedEvent(int button)
        :m_button(button) {}
    
    int MouseButtonPressedEvent::button() const
    {
        return m_button;
    }

    EventType MouseButtonPressedEvent::staticType()
    {
        return EventType::MouseButtonPressed;
    }

    EventType MouseButtonPressedEvent::eventType() const
    {
        return staticType();
    }

    const char* MouseButtonPressedEvent::name() const
    {
        return "MouseButtonPressed";
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button)
        :m_button(button) {}

    int MouseButtonReleasedEvent::button() const
    {
        return m_button;
    }

    EventType MouseButtonReleasedEvent::staticType()
    {
        return EventType::MouseButtonReleased;
    }

    EventType MouseButtonReleasedEvent::eventType() const
    {
        return staticType();
    }

    const char* MouseButtonReleasedEvent::name() const
    {
        return "MouseButtonReleased";
    }

    /*KEYBOARD RELATED EVENTS*/

    KeyPressedEvent::KeyPressedEvent(int button)
        :m_button(button) {}

    int KeyPressedEvent::button() const
    {
        return m_button;
    }

    EventType KeyPressedEvent::staticType()
    {
        return EventType::KeyPressed;
    }

    EventType KeyPressedEvent::eventType() const
    {
        return staticType();
    }

    const char* KeyPressedEvent::name() const
    {
        return "KeyPressed";
    }

    KeyReleasedEvent::KeyReleasedEvent(int button)
        :m_button(button) {}

    int KeyReleasedEvent::button() const
    {
        return m_button;
    }

    EventType KeyReleasedEvent::staticType()
    {
        return EventType::KeyReleased;
    }

    EventType KeyReleasedEvent::eventType() const
    {
        return staticType();
    }

    const char* KeyReleasedEvent::name() const
    {
        return "KeyReleased";
    }

    /*WINDOW EVENTS*/

    WindowResizeEvent::WindowResizeEvent(uint32_t width, uint32_t height)
        : m_width(width)
        , m_height(height)
    {
    }

    uint32_t WindowResizeEvent::width() const
    {
        return m_width;
    }
    uint32_t WindowResizeEvent::height() const
    {
        return m_height;
    }

    EventType WindowResizeEvent::staticType()
    {
        return EventType::WindowResize;
    }

    EventType WindowResizeEvent::eventType() const
    {
        return staticType();
    }

    const char* WindowResizeEvent::name() const
    {
        return "WindowResize";
    }

    EventType WindowCloseEvent::staticType()
    {
        return EventType::WindowClose;
    }
    EventType WindowCloseEvent::eventType() const
    {
        return staticType();
    }

    const char* WindowCloseEvent::name() const
    {
        return "WindowClose";
    }
}

