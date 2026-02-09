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
    for(auto entity : ent) {
        if (ECS::hasComponent<Vertices>(entity))
            std::println("has component");

        auto comp = ECS::getComponent<Vertices>(entity);

        glCreateVertexArrays(1, &comp.m_vao);
        glCreateBuffers(1, &comp.m_vbo);
        glCreateBuffers(1, &comp.m_ebo);

        glBindVertexArray(comp.m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, comp.m_vbo);
        glBufferData(GL_ARRAY_BUFFER,
                comp.m_vertices.size() * sizeof(Vertex),
                comp.m_vertices.data(),
                GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp.m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                comp.m_indices.size() * sizeof(uint32_t),
                comp.m_indices.data(),
                GL_STATIC_DRAW
        );

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
       
        // glNamedBufferData(comp.m_vao,
        //         comp.m_vertices.size() * sizeof(Vertex),
        //         comp.m_vertices.data(),
        //         GL_STATIC_DRAW
        // );
        //
        // glNamedBufferData(comp.m_ebo,
        //         comp.m_indices.size() * sizeof(uint32_t),
        //         comp.m_indices.data(),
        //         GL_STATIC_DRAW
        // );

        // glVertexArrayVertexBuffer(comp.m_vao, 0, comp.m_vbo, 0, 3 * sizeof(GLfloat));
        // glEnableVertexArrayAttrib(comp.m_vao, 0);
        // glVertexArrayAttribBinding(comp.m_vao, 0, 0);
        // glVertexArrayAttribFormat(comp.m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
        // glEnableVertexArrayAttrib(comp.m_vao, 1);
        // glVertexArrayAttribBinding(comp.m_vao, 1, 0);
        // glVertexArrayAttribFormat(comp.m_vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        // glVertexArrayElementBuffer(comp.m_vao, comp.m_vbo);
    }
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
    m_uilayer->begin();
    m_uilayer->drawViewport(m_renderer);
    m_uilayer->end();
}

