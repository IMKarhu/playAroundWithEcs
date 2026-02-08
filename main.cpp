#include "window.h"
#include "renderSystem.h"
#include "imguiLayer.h"
#include "components.h"
#include "ecs.h"
#include <cassert>
#include <cstdio>
#include <print>

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

    for(size_t i = 0; i < MAXENTITIES; ++i) {
        // std::println("i: {}", i);
        Entity ent = 0;
        m_entities.push_back(ent);
        ent++;
    }

    for(size_t i = 0; i < m_entities.size(); ++i) {
        ECS::addComponent<Transform>(i, {{0,0,0},{0,0,0},{1,1,1}});
    }

    Vertices verts;
    verts.m_vertices = {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.0f, 0.5f, 0.0f}
    };
    verts.m_indices = {
        0, 1, 2
    };
    ECS::addComponent<Vertices>(m_entities[0], verts);

    auto transformPool = ECS::getComponentPool<Transform>();
    auto entt = transformPool->entities();
    std::println("size of entities with transform component {}", entt.size());
    auto vertsPool = ECS::getComponentPool<Vertices>();
    entt = vertsPool->entities();
    std::println("size of entities with vertices component {}", entt.size());

    bool demoWindow = true;
    bool testCheckbox = false;
    float testSlider = 0.f;
    renderSystem.initialize();
    while(!window.shouldClose()) {

        renderSystem.update(10.0f);
        renderSystem.renderui();

        // ImGui::ShowDemoWindow(&demoWindow);
        // // uilayer.drawViewport(renderer);
        //
        // ImGui::Begin("settings");
        // ImGui::Text("testing text");
        // ImGui::Checkbox("test checkbox", &testCheckbox);
        // ImGui::SliderFloat("test slider", &testSlider, 0.0f, 1.0f);
        // ImGui::End();

        // uilayer.end();

        window.pollEvents();
        window.swapBuffers();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
