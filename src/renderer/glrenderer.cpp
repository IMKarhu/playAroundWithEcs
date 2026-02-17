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
        // if (ECS::hasComponent<Vertices>(entity))
        //     std::println("has component");

        auto& comp = ECS::getComponent<Vertices>(entity);

        glCreateVertexArrays(1, &comp.m_vao);
        glCreateBuffers(1, &comp.m_vbo);
        glCreateBuffers(1, &comp.m_ebo);

        glNamedBufferData(comp.m_vbo,
                comp.m_vertices.size() * sizeof(Vertex),
                comp.m_vertices.data(),
                GL_STATIC_DRAW
        );

        glNamedBufferData(comp.m_ebo,
                comp.m_indices.size() * sizeof(uint32_t),
                comp.m_indices.data(),
                GL_STATIC_DRAW
        );

        glVertexArrayVertexBuffer(comp.m_vao, 0, comp.m_vbo, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(comp.m_vao, comp.m_ebo);

        glEnableVertexArrayAttrib(comp.m_vao, 0);
        glVertexArrayAttribFormat(comp.m_vao,
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                offsetof(Vertex, position)
        );
        glVertexArrayAttribBinding(comp.m_vao, 0, 0);

        glEnableVertexArrayAttrib(comp.m_vao, 1);
        glVertexArrayAttribFormat(comp.m_vao,
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                offsetof(Vertex, color)
        );
        glVertexArrayAttribBinding(comp.m_vao, 1, 0);
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
    auto& entt = pool->entities();

    m_shader->use();
    auto& cameraTrans = ECS::getComponent<Transform>(0);
    auto& cameramodel = ECS::getComponent<model>(0);
    auto& cam = ECS::getComponent<Camera>(0);

    //temporary
    if(glfwGetKey(m_window.handle(), GLFW_KEY_S) == GLFW_PRESS)
        cameraTrans.position.z -= 1.0 * dt;
    if(glfwGetKey(m_window.handle(), GLFW_KEY_W) == GLFW_PRESS)
        cameraTrans.position.z += 1.0 * dt;

    cam.view = glm::lookAt(cameraTrans.position, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0));
    cam.proj = glm::perspective(glm::radians(45.0f), (float)fb.framebufferSpec().width/(float)fb.framebufferSpec().height, 0.001f, 1000.0f);

    for(auto entity : entt) {
        auto& trans = ECS::getComponent<Transform>(entity);
        auto& comp = ECS::getComponent<Vertices>(entity);
        auto& m = ECS::getComponent<model>(entity);
        m.model = glm::translate(glm::mat4(1), trans.position);
        m.model = glm::rotate(m.model, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
        m.model = glm::scale(m.model, glm::vec3(trans.scale));
        auto mvp =  cam.proj * cam.view * m.model;
        m_shader->setUniformMat4("u_mvp", mvp);

        glBindVertexArray(comp.m_vao);
        glDrawElements(GL_TRIANGLES, comp.m_indices.size(), GL_UNSIGNED_INT, 0);
    }
}

void GLRenderer::endFrame()
{
    auto& fb = *m_viewportfb;
    fb.unbind();
}

FrameBuffer &GLRenderer::viewportfb()
{
    return *m_viewportfb;
}
