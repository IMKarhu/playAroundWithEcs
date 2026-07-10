#include "application.h"
#include "opengl/glrenderer.h"
#include "windowing/eventDispatcher.h"
#include "gameLayer.h"
#include "scene.h"
#include <chrono>

Application::Application(Platform platform)
{
    m_window = std::make_unique<Lumos::Window>();
    m_window->setEventCallback(std::bind(&Application::event, this, std::placeholders::_1));
    //this can be glrenderer for now since thats all we have
    m_renderer = std::make_shared<GLRenderer>();
    //quick fix for setting framebuffer width and height so I dont need to pass window to renderer
    m_renderer->setDefaultFramebufferDimensios(m_window->width(), m_window->height());
    m_assetmanager = std::make_unique<Lumos::AssetManager>(m_renderer->getGraphicsDevice());

    pushLayer(std::make_unique<GameLayer>(*m_renderer, *m_assetmanager, *m_window));
    m_renderer->createAndAddToShaderCache("screen","shaders/screen.vert", "shaders/screen.frag");
    m_renderer->createAndAddToShaderCache("basePass", "shaders/shader.vert", "shaders/shader.frag");
    m_renderer->createAndAddToShaderCache("lighting", "shaders/lighting.vert", "shaders/lighting.frag");

    auto Scene = std::make_shared<Lumos::Scene>(*m_assetmanager, "test scene");
    m_scenemanager.setActiveScene(Scene);
}

void Application::run()
{
    auto curTime = std::chrono::high_resolution_clock::now();
    while(!m_window->shouldClose()) {
        auto startTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::seconds::period>(startTime - curTime).count();

        for (auto& layer : m_layers) {
            layer->update(dt, m_scenemanager.getActiveScene());
        }

        m_window->pollEvents();
        m_window->swapBuffers();
    }
}

void Application::pushLayer(std::unique_ptr<Lumos::Layer> layer)
{
    layer->attach();
    m_layers.push_back(std::move(layer));
}

void Application::event(Lumos::Event& event)
{
    Lumos::EventDispatcher dispatcher(event);

    dispatcher.dispatch<Lumos::WindowCloseEvent>([this](Lumos::WindowCloseEvent& e) {
            m_window->setWindowShouldClose(true);
            return true;
    });

    for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it) {
        if (event.handled) {
            break;
        }
        (*it)->event(event, m_scenemanager.getActiveScene());
    }
}
