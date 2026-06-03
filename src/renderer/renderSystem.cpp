#include "renderSystem.h"
#include "ecsImpl/ecs.h"
#include "ecsImpl/components.h"
#include "opengl/glrenderer.h"
#include <print>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem() {}

void RenderSystem::initialize(const Ecs &ecs, const Renderer &renderer)
{
    std::println("initialize");
    for (auto ent : ecs.view<Mesh>()) {
        auto& mesh = ecs.getComponent<Mesh>(ent);
        renderer.createMeshPrimitive(mesh.name, mesh.vertices, mesh.indices, mesh.vbo, mesh.ebo);
    }
}

void RenderSystem::update(float dt, const Ecs &ecs, const Renderer &renderer)
{
    renderer.beginFrame();
    for (auto ent : ecs.view<Transform, Mesh>()) {
        auto& transform = ecs.getComponent<Transform>(ent);
        auto& mesh = ecs.getComponent<Mesh>(ent);
        RenderInfo renderinfo;
        renderinfo.shadername = "main";
        renderinfo.meshname = mesh.name;
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0));
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)600/(float)400, 0.001f, 1000.0f);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position);
        model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
        model = glm::scale(model, glm::vec3(transform.scale));

        auto mvp = proj * view * model;
        renderinfo.transform = mvp;
        renderer.renderScene(renderinfo);
    }

    renderer.endFrame();
}
