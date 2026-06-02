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
        // glCreateVertexArrays(1, &mesh.vao);
        // glCreateBuffers(1, &mesh.vbo);
        // glCreateBuffers(1, &mesh.ebo);
        //
        // glNamedBufferData(mesh.vbo,
        //         mesh.vertices.size() * sizeof(Vertex),
        //         mesh.vertices.data(),
        //         GL_STATIC_DRAW
        // );
        //
        // glNamedBufferData(mesh.ebo,
        //         mesh.indices.size() * sizeof(uint32_t),
        //         mesh.indices.data(),
        //         GL_STATIC_DRAW
        // );
        //
        // glVertexArrayVertexBuffer(mesh.vao, 0, mesh.vbo, 0, sizeof(Vertex));
        // glVertexArrayElementBuffer(mesh.vao, mesh.ebo);
        //
        // glEnableVertexArrayAttrib(mesh.vao, 0);
        // glVertexArrayAttribFormat(mesh.vao,
        //         0,
        //         3,
        //         GL_FLOAT,
        //         GL_FALSE,
        //         offsetof(Vertex, position)
        // );
        // glVertexArrayAttribBinding(mesh.vao, 0, 0);
        //
        // glEnableVertexArrayAttrib(mesh.vao, 1);
        // glVertexArrayAttribFormat(mesh.vao,
        //         1,
        //         3,
        //         GL_FLOAT,
        //         GL_FALSE,
        //         offsetof(Vertex, color)
        // );
        // glVertexArrayAttribBinding(mesh.vao, 1, 0);
    }

    //auto entities = scene->getAllEntitiesWithComponent<Vertices>();
    // auto& ecs = scene->getEcs();
    // Entity ent = ecs.createEntity("entity1");
    // Vertices verts;
    // verts.m_vertices = {
    // };
    // verts.m_indices = {
    // };
    //
    // ecs.addComponent<Vertices>(ent, verts);
    // for (auto entity : ecs.view<Vertices>()) {
    //     auto& vert = ecs.getComponent<Vertices>(entity);
    // }

    // auto pool = ECS::getComponentPool<Vertices>();
    // auto ent = pool->entities();
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
    // std::println("update rendersystem");
    // std::println("number of all entities: {}", ecs.getNumOfAllEntities());
    // m_framebufferManager->renderto(m_fb, [&](){
    //         m_renderer->renderScene(dt, m_fb->framebufferSpec().width, m_fb->framebufferSpec().height, scene);
    // });
}
