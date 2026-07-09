#pragma once
#include <functional>
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

//there's probably more event types, but these came to my mind first.
enum class EventType
{
    WindowResize,
    KeyPress,
    MouseMove
};

struct Event
{
    EventType type;
};

struct ResizeEvent : public Event
{
    uint32_t width;
    uint32_t height;
    ResizeEvent(uint32_t w, uint32_t h)
        : width(w), height(h)
    {
        type = EventType::WindowResize;
    }
};

struct KeyEvent : public Event
{
    uint32_t key;
    uint32_t action;
    uint32_t mods;
    KeyEvent(uint32_t k, uint32_t a, uint32_t m)
        : key(k), action(a), mods(m)
    {
        type = EventType::KeyPress;
    }

};

class CORE_API EventDispatcher
{
public:
    using Callback = std::function<void(const Event&)>;

    static void subscribe(EventType type, Callback callback);
    static void sendEvent(const Event& event);
private:
};
