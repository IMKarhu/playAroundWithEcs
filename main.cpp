#include "window.h"
#include "renderSystem.h"
#include "imguiLayer.h"
#include "components.h"
#include "ecs.h"
#include <cassert>
#include <cstdio>
#include <print>
#include <chrono>


//Fine for now to limit max amount.
//In reality there should not be any kind of entity limitations
const uint32_t MAXENTITIES = 5000;


int main()
{
    Window window;
    RenderSystem renderSystem(window);
    bool ok = ECS::registerComponentPool<Transform>();
    assert(ok);
    ok = ECS::registerComponentPool<Sprite>();
    assert(ok);
    ok = ECS::registerComponentPool<Vertices>();
    assert(ok);
    ok = ECS::registerComponentPool<Camera>();
    assert(ok);

    for(size_t i = 0; i < MAXENTITIES; ++i) {
        Entity ent = 0;
        m_entities.push_back(ent);
        ent++;
    }

    for(size_t i = 0; i < m_entities.size(); ++i) {
        ECS::addComponent<Transform>(i, {{0,0,0},{0,0,0},{1,1,1}});
    }

    Vertices verts;
    verts.m_vertices = {
        // front
        {{-0.5f, -0.5f,  0.5f}, {1, 0, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 1, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}},
        {{-0.5f,  0.5f,  0.5f}, {1, 1, 1}},
        // back
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}},
        {{-0.5f, -0.5f, -0.5f}, {0, 1, 0}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {1, 1, 1}},
        // left
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
        {{-0.5f, -0.5f,  0.5f}, {0, 1, 0}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}},
        {{-0.5f,  0.5f, -0.5f}, {1, 1, 1}},
        // right
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {1, 1, 1}},
        // top
        {{-0.5f,  0.5f,  0.5f}, {1, 0, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}},
        {{-0.5f,  0.5f, -0.5f}, {1, 1, 1}},
        // bottom
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}},
        {{-0.5f, -0.5f,  0.5f}, {1, 1, 1}},
    };
    verts.m_indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    ECS::addComponent<Vertices>(m_entities[0], verts);
    Camera camera = {
        .model = glm::mat4(1),
        .view = glm::mat4(1),
        .proj = glm::mat4(1),
        .position = glm::vec3(0.0f, 0.0f, 5.0f)
    };
    ECS::addComponent<Camera>(m_entities[0], camera);

    auto transformPool = ECS::getComponentPool<Transform>();
    auto entt = transformPool->entities();
    std::println("size of entities with transform component {}", entt.size());
    auto vertsPool = ECS::getComponentPool<Vertices>();
    entt = vertsPool->entities();
    std::println("size of entities with vertices component {}", entt.size());

    renderSystem.initialize();
    auto curTime = std::chrono::high_resolution_clock::now();
    while(!window.shouldClose()) {
        auto startTime = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float, std::chrono::seconds::period>(startTime - curTime).count();

        renderSystem.update(dt);
        renderSystem.renderui();

        window.pollEvents();
        window.swapBuffers();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}

