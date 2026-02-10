#include "glrenderer.h"
#include "glframebuffer.h"
#include "shader.h"
#include "window.h"
#include "components.h"
#include <glm/gtc/matrix_transform.hpp>


GLRenderer::GLRenderer(Window& window)
    : m_window(window)
{
    FramebufferSpec spec = {
        .width = window.width(),
        .height = window.height()
    };
    m_viewportfb = std::make_unique<GLFramebuffer>(spec);
    m_shader = std::make_unique<Shader>("../shader.vert", "../shader.frag");
}

GLRenderer::~GLRenderer() {}

void GLRenderer::initialize(std::vector<Entity>& entities)
{
    for(auto entity : entities) {
        if (ECS::hasComponent<Vertices>(entity))
            std::println("has component");

        auto comp = ECS::getComponent<Vertices>(entity);

        glCreateVertexArrays(1, &comp.m_vao);
        glCreateBuffers(1, &comp.m_vbo);
        glCreateBuffers(1, &comp.m_ebo);

        glBindVertexArray(comp.m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, comp.m_vbo);
        glNamedBufferData(comp.m_vao,
                comp.m_vertices.size() * sizeof(Vertex),
                comp.m_vertices.data(),
                GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp.m_ebo);
        glNamedBufferData(comp.m_ebo,
                comp.m_indices.size() * sizeof(uint32_t),
                comp.m_indices.data(),
                GL_STATIC_DRAW
        );

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        glEnableVertexAttribArray(1);

    }

}

void GLRenderer::beginFrame()
{
    auto& fb = *m_viewportfb;
    fb.bind();
    glViewport(0, 0, fb.framebufferSpec().width, fb.framebufferSpec().height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::renderScene(float dt)
{
    auto& fb = *m_viewportfb;
    auto pool = ECS::getComponentPool<Vertices>();
    auto entt = pool->entities();
    m_shader->use();
    for(auto entity : entt) {
        if (ECS::hasComponent<Camera>(entity)) {
            auto trans = ECS::getComponent<Transform>(entity);
            // trans.rotation.x= 1.0 * dt;
            float rot = 20.0f * dt;
            // std::println("entity id {} has camera", entity);
            // std::println("entity rotation x {}", trans.rotation.x);
            auto cam = ECS::getComponent<Camera>(entity);
            cam.model = glm::mat4(1);
            cam.model = glm::translate(cam.model, trans.position);
            cam.model = glm::rotate(cam.model, glm::radians(rot), glm::vec3(0.0, 1.0, 0.0));
            cam.model = glm::scale(cam.model, glm::vec3(trans.scale));
            cam.view = glm::lookAt(cam.position, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0));
            cam.proj = glm::perspective(glm::radians(45.0f), (float)fb.framebufferSpec().width/(float)fb.framebufferSpec().height, 0.1f, 100.0f);
            auto mvp =  cam.proj * cam.view * cam.model;
            m_shader->setUniformMat4("u_mvp", mvp);
        }

        auto comp = ECS::getComponent<Vertices>(entity);
        glBindVertexArray(comp.m_vao);
        glDrawElements(GL_TRIANGLES, comp.m_indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void GLRenderer::endFrame()
{
    auto& fb = *m_viewportfb;
    fb.unbind();
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

FrameBuffer &GLRenderer::viewportfb()
{
    return *m_viewportfb;
}
