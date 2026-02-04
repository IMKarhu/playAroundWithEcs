#include "GLFW/glfw3.h"
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"
#include "imgui.h"
#include "window.h"
#include "ecs.h"
#include <cassert>
#include <cstdio>
#include <array>
#include <print>


//Fine for now to limit max amount.
//In reality there should not be any kind of entity limitations
const uint32_t MAXENTITIES = 5000;

struct Transform
{
    std::array<float, 3> m_position;
    std::array<float, 3> m_rotation;
    std::array<float, 3> m_scale;
};

struct Sprite
{
    float a;
};


int main()
{
    Window window;
    bool ok = ECS::registerComponentPool<Transform>();
    assert(ok);
    ok = ECS::registerComponentPool<Sprite>();
    assert(ok);

    for(size_t i = 0; i < 4000; ++i) {
        Entity ent = 1;
        m_entities.push_back(ent);
        ent++;
    }

    for(size_t i = 0; i < m_entities.size(); ++i) {
        ECS::addComponent<Transform>(i, {{0,0,0},{0,0,0},{1,1,1}});
    }
    auto transformPool = ECS::getComponentPool<Transform>();
    auto entt = transformPool->entities();
    std::println("size of entities {}", entt.size());

    bool demoWindow = true;
    while(!window.shouldClose()) {
        window.pollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(&demoWindow);
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar; //| ImGuiWindowFlags_NoDocking;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGui::Begin("DockSpace demo", &demoWindow, windowFlags);
        ImGui::PopStyleVar(3);
        ImGui::DockSpace(ImGui::GetID("DockSpace"));
        ImGui::End();
        ImGui::Render();

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLFWwindow *backupcontext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupcontext);
        // ImGui::EndFrame();

        window.swapBuffers();
    }
    return 0;
}
