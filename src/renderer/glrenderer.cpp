#include "glrenderer.h"
#include "ecs.h"
#include "shader.h"
#include "window.h"
#include "components.h"
#include <glm/gtc/matrix_transform.hpp>


GLRenderer::GLRenderer(Window& window)
    : m_window(window)
{
    m_shader = std::make_unique<Shader>("../../shader.vert", "../../shader.frag");
}

GLRenderer::~GLRenderer() {}

void GLRenderer::initialize()
{
}

void GLRenderer::beginFrame()
{

}

void GLRenderer::renderScene(float dt, uint32_t width, uint32_t height)
{
    // auto pool = ECS::getComponentPool<Vertices>();
    // auto& entt = pool->entities();

    // m_shader->use();
    // auto& cameraTrans = ECS::getComponent<Transform>(0);
    // auto& cameramodel = ECS::getComponent<model>(0);
    // auto& cam = ECS::getComponent<Camera>(0);

    //temporary
    // if(glfwGetKey(m_window.handle(), GLFW_KEY_S) == GLFW_PRESS)
    //     cameraTrans.position.z -= 1.0 * dt;
    // if(glfwGetKey(m_window.handle(), GLFW_KEY_W) == GLFW_PRESS)
    //     cameraTrans.position.z += 1.0 * dt;
    //
    // cam.view = glm::lookAt(cameraTrans.position, glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0));
    // cam.proj = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.001f, 1000.0f);
    //
    // for(auto entity : entt) {
    //     auto& trans = ECS::getComponent<Transform>(entity);
    //     auto& comp = ECS::getComponent<Vertices>(entity);
    //     auto& m = ECS::getComponent<model>(entity);
    //     m.model = glm::translate(glm::mat4(1), trans.position);
    //     m.model = glm::rotate(m.model, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
    //     m.model = glm::scale(m.model, glm::vec3(trans.scale));
    //     auto mvp =  cam.proj * cam.view * m.model;
    //     m_shader->setUniformMat4("u_mvp", mvp);
    //
    //     glBindVertexArray(comp.m_vao);
    //     glDrawElements(GL_TRIANGLES, comp.m_indices.size(), GL_UNSIGNED_INT, 0);
    // }
}

void GLRenderer::endFrame()
{
}
