#pragma once
#include "layer.h"
#include "windowing/window.h"
#include "renderer.h"
#include <vector>
#include <memory>

class Application
{
public:
    Application(Platform platform);
    void run();
    void pushLayer(std::unique_ptr<Layer> layer);
private:
    std::vector<std::unique_ptr<Layer>> m_layers;
    std::unique_ptr<Window> m_window;
    std::shared_ptr<Renderer> m_renderer;
};
