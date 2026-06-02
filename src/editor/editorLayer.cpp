#include "editorLayer.h"
#include "windowing/window.h"
#include <print>
#include <iostream>

// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"


EditorLayer::EditorLayer(const Renderer &renderer, Window &window)
    : m_renderer(renderer), m_window(window), Layer("Editor Layer")
{
}

EditorLayer::~EditorLayer() {}

void EditorLayer::attach()
{
//     std::println("EditorLayer attached");
//     //everything is from glfw example
//     //https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//
//     ImGuiIO& io = ImGui::GetIO();
//     //TODO: fix this on linux, for some reason imgui context is null deep in imgui files 
// #ifdef _WIN32
//     float main_scale = ImGui_ImplGlfw_GetContentScaleForWindow(m_window.handle());
// #endif
//     (void)io;
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//     io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable docking
//     io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable multi-viewports
//
//     ImGui::StyleColorsDark();
//     ImGuiStyle& style = ImGui::GetStyle();
// #ifdef _WIN32
//     style.ScaleAllSizes(main_scale);
//     style.FontScaleDpi = main_scale;
// #endif
//     io.ConfigDpiScaleFonts = true;
//     io.ConfigDpiScaleViewports = true;
//
//     if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
//         style.WindowRounding = 0.0f;
//         style.Colors[ImGuiCol_WindowBg].w = 1.0f;
//     }
//
//     const char* glsl_version = "#version 460";
//     ImGui_ImplGlfw_InitForOpenGL(m_window.handle(), true);
//     ImGui_ImplOpenGL3_Init(glsl_version);
}
void EditorLayer::detach() {}
void EditorLayer::update(float dt)
{
    // ImGui_ImplOpenGL3_NewFrame();
    // ImGui_ImplGlfw_NewFrame();
    // ImGui::NewFrame();
    //
    //
    // bool demoWindow = true;
    // bool testCheckbox = false;
    // float testSlider = 0.f;
    //
    // ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    // ImGuiViewport* viewport = ImGui::GetMainViewport();
    // ImGui::SetNextWindowPos(viewport->Pos);
    // ImGui::SetNextWindowSize(viewport->Size);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    // windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    // ImGui::Begin("DockSpace demo", &demoWindow, windowFlags);
    // ImGui::PopStyleVar(3);
    // ImGui::DockSpace(ImGui::GetID("DockSpace"));
    // ImGui::End();
    // /*viewport*/
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    // ImGui::Begin("Viewport", &demoWindow, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    // ImGui::PopStyleVar(2);
    // ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    // // if(viewportSize.x > 0 && viewportSize.y > 0)
    // //     framebuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
    // //
    // // ImGui::Image((ImTextureID)(intptr_t)framebuffer->colorAttachment(), viewportSize, ImVec2{0,1},ImVec2{1,0});
    // ImGui::End();
    //
    // auto io = ImGui::GetIO();
    // ImGui::Render();
    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
    //     GLFWwindow *backupcontext = glfwGetCurrentContext();
    //     ImGui::UpdatePlatformWindows();
    //     ImGui::RenderPlatformWindowsDefault();
    //     m_window.makeCtxCurrent(backupcontext);
    // }
}
void EditorLayer::event(Event &event)
{
}

