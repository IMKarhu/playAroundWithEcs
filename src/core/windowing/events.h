#pragma once
#include <cstdint>

#ifdef _WIN32
    #ifdef CORE_EXPORT
    #define CORE_API __declspec(dllexport)
    #else
    #define CORE_API __declspec(dllimport)
    #endif
#else
    #define CORE_API
#endif

namespace Lumos
{

    //there's probably more event types, but these came to my mind first.
    enum class EventType
    {
        WindowClose,
        WindowResize,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved
    };

    class CORE_API Event
    {
    public:
        virtual ~Event() = default;

        virtual EventType eventType() const = 0;
        virtual const char* name() const = 0;
        bool handled = false;
    };

    class CORE_API MouseButtonPressedEvent : public Event
    {
    public:
        MouseButtonPressedEvent(int button);

        int button() const;
        static EventType staticType();
        virtual EventType eventType() const override;
        virtual const char* name() const override;
    private:
        int m_button;
    };

    class CORE_API MouseButtonReleasedEvent : public Event
    {
    public:
        MouseButtonReleasedEvent(int button);

        int button() const;
        static EventType staticType();
        virtual EventType eventType() const override;
        virtual const char* name() const override;
    private:
        int m_button;
    };

    class CORE_API KeyPressedEvent : public Event
    {
    public:
        KeyPressedEvent(int button);

        int button() const;
        static EventType staticType();
        virtual EventType eventType() const override;
        virtual const char* name() const override;
    private:
        int m_button;
    };

    class CORE_API KeyReleasedEvent : public Event
    {
    public:
        KeyReleasedEvent(int button);

        int button() const;
        static EventType staticType();
        virtual EventType eventType() const override;
        virtual const char* name() const override;
    private:
        int m_button;
    };

    class CORE_API WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(uint32_t width, uint32_t height);

        uint32_t width() const;
        uint32_t height() const;
        static EventType staticType();
        virtual EventType eventType() const override;
        virtual const char* name() const override;
    private:
        uint32_t m_width;
        uint32_t m_height;
    };

    class CORE_API WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        int button() const;
        static EventType staticType();
        virtual EventType eventType() const override;
        virtual const char* name() const override;
    private:
    };


}//namespace Lumos

