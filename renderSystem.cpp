#include "renderSystem.h"
#include "window.h"
#include "glrenderer.h"
#include "imguiLayer.h"
#include "shader.h"
#include "components.h"
#include "ecs.h"

RenderSystem::RenderSystem(Window& window)
    : m_window(window)
{
    m_renderer = std::make_unique<GLRenderer>(window);
    m_uilayer = std::make_unique<ImguiLayer>(window);
    m_shader = std::make_unique<Shader>("../shader.vert", "../shader.frag");
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
    auto pool = ECS::getComponentPool<Vertices>();
    auto entt = pool->entities();
    m_shader->use();
    for(auto entity : entt) {
        auto comp = ECS::getComponent<Vertices>(entity);
        glBindVertexArray(comp.m_vao);
        glDrawElements(GL_TRIANGLES, comp.m_indices.size(), GL_UNSIGNED_INT, 0);
    }
    m_renderer->renderScene();
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

