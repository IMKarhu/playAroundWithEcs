#include "application.h"
#include "opengl/glrenderer.h"
#include "gameLayer.h"
#include "editor/editorLayer.h"
#include <chrono>

Application::Application(Platform platform)
{
    m_window = std::make_unique<Window>();
    //this can be glrenderer for now since thats all we have
    m_renderer = std::make_shared<GLRenderer>(*m_window);

    pushLayer(std::make_unique<GameLayer>(*m_renderer));
    m_renderer->createAndAddToShaderCache("main", "../src/game/shaders/shader.vert", "../src/game/shaders/shader.frag");

    //temporary for closing application by pressing ESC
    EventDispatcher::subscribe(EventType::KeyPress, [this](const Event &e) {
        const auto &event = static_cast<const KeyEvent&>(e);
        if (event.key == 256) {
            m_window->setWindowShouldClose(true);
        }
    });


}

void Application::run()
{
    auto curTime = std::chrono::high_resolution_clock::now();
    while(!m_window->shouldClose()) {
        auto startTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::seconds::period>(startTime - curTime).count();

        for (auto& layer : m_layers) {
            layer->update(dt);
        }

        m_window->pollEvents();
        m_window->swapBuffers();
    }
}

void Application::pushLayer(std::unique_ptr<Layer> layer)
{
    layer->attach();
    m_layers.push_back(std::move(layer));
}
