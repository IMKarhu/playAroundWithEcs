#pragma once
#include "layer.h"
#include "windowing/window.h"
#include "renderer.h"
#include "assetmanagers/assetManager.h"
#include "scene/sceneManager.h"
#include "windowing/events.h"
#include <vector>
#include <memory>

class Application
{
public:
    Application(Platform platform);
    void run();
    void pushLayer(std::unique_ptr<Lumos::Layer> layer);
    void event(Lumos::Event& event);
private:
    std::vector<std::unique_ptr<Lumos::Layer>> m_layers;
    std::unique_ptr<Lumos::Window> m_window;
    std::shared_ptr<Renderer> m_renderer;
    std::unique_ptr<Lumos::AssetManager> m_assetmanager;
    Lumos::SceneManager m_scenemanager;
};
