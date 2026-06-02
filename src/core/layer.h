#pragma once
#include <string>
#include "windowing/events.h"

class Renderer;

class Layer
{
public:
    Layer(const std::string &name = "defaultLayer")
        : m_layerName(name) {}
    virtual ~Layer() = default;
    virtual void attach() {}
    virtual void detach() {}
    virtual void update(float dt) {}
    virtual void editorUpdate() {}
    virtual void event(Event &event) {}
private:
    std::string m_layerName;
};
