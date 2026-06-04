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
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    for (auto ent : ecs.view<Transform, Camera>()) {
        auto& camtransform = ecs.getComponent<Transform>(ent);
        auto& camera = ecs.getComponent<Camera>(ent);
        if (camera.primary) {

            view = glm::lookAt(camtransform.position, camtransform.position + glm::vec3(0.0f,0.0f, -1.0f), camera.upvector);
            proj = glm::perspective(glm::radians(camera.fov), camera.aspectratio, camera.nearclip, camera.farclip);
            break;
        }
    }

    renderer.beginFrame();

    for (auto ent : ecs.view<Transform, Mesh>()) {
        auto& transform = ecs.getComponent<Transform>(ent);
        auto& mesh = ecs.getComponent<Mesh>(ent);
        RenderInfo renderinfo;
        renderinfo.shadername = "main";
        renderinfo.meshname = mesh.name;
        transform.model = glm::translate(glm::mat4(1.0f), transform.position);
        transform.model = glm::rotate(transform.model, glm::radians(20.0f * dt), glm::vec3(0.0, 1.0, 0.0));
        transform.model = glm::scale(transform.model, glm::vec3(transform.scale));

        auto mvp = proj * view * transform.model;
        renderinfo.transform = mvp;
        renderer.renderScene(renderinfo);
    }

    renderer.endFrame();
}
