#include "renderSystem.h"
#include "window.h"
#include "glrenderer.h"
#include "imguiLayer.h"
#include "components.h"
#include "ecs.h"

RenderSystem::RenderSystem(Window& window)
    : m_window(window)
{
    m_renderer = std::make_unique<GLRenderer>(window);
    m_uilayer = std::make_unique<ImguiLayer>(window);
}

RenderSystem::~RenderSystem() {}

void RenderSystem::initialize()
{
    std::println("initialize");
    auto pool = ECS::getComponentPool<Vertices>();
    auto ent = pool->entities();
    m_renderer->initialize(ent);
}

void RenderSystem::update(float dt)
{
    m_renderer->beginFrame();
    m_renderer->renderScene(dt);
    m_renderer->endFrame();
}

void RenderSystem::renderui()
{
    static bool open = true;
    m_uilayer->begin();
    m_uilayer->drawViewport(m_renderer);
    ImguiLayer::beginWindow("settings", &open);
    ImguiLayer::endWindow();
    m_uilayer->end();


}

