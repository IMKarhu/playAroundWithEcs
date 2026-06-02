#include "imguiLayer.h"
#include "windowing/window.h"
#include "renderer.h"
#include "framebuffer.h"
#include "ecsImpl/components.h"

#include <print>

ImguiLayer::ImguiLayer(Window& window)
    : m_window(window)
{
}
ImguiLayer::~ImguiLayer()
{
}

void ImguiLayer::begin()
{
}

void ImguiLayer::end()
{
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

void ImguiLayer::drawViewport(std::shared_ptr<FrameBuffer> framebuffer)
{
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
    // // ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    // // ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiCond_FirstUseEver);
    // ImGui::Begin("DockSpace demo", &demoWindow, windowFlags);
    // ImGui::PopStyleVar(3);
    // ImGui::DockSpace(ImGui::GetID("DockSpace"));
    // ImGui::End();
    // /*viewport*/
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    // ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    // ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiCond_FirstUseEver);
    // ImGui::Begin("Viewport", &demoWindow, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    // ImGui::PopStyleVar(2);
    // ImVec2 viewportSize = ImGui::GetContentRegionAvail();
    // if(viewportSize.x > 0 && viewportSize.y > 0)
    //     framebuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
    //
    // ImGui::Image((ImTextureID)(intptr_t)framebuffer->colorAttachment(), viewportSize, ImVec2{0,1},ImVec2{1,0});
    // ImGui::End();
}

void ImguiLayer::drawSceneGraph()
{
    // ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiCond_FirstUseEver);
    // ImGui::Begin("Scene hierarchy");
    // auto& ecs = scene->getEcs();
    //
    // for (auto entity : ecs.view<Id>()) {
    //     auto& id = ecs.getComponent<Id>(entity);
    //
    //     // ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
    //     ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf;
    //
    //     if (m_selectedEntity == entity) {
    //         flags |= ImGuiTreeNodeFlags_Selected;
    //     }
    //     flags |= ImGuiTreeNodeFlags_Leaf;
    //
    //     bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, id.name.c_str());
    //
    //     if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
    //         m_selectedEntity = entity;
    //     }
    //
    //     if (ImGui::BeginPopupContextItem()) {
    //         // m_selectedEntity = entity;
    //
    //         if (ImGui::MenuItem("Rename")) {
    //             /* TODO logic */
    //         }
    //
    //         ImGui::Separator();
    //
    //         if (ImGui::BeginMenu("Add Component")) {
    //             if (ImGui::MenuItem("Transform")) {
    //                 Transform trans{};
    //                 ecs.addComponent<Transform>(entity, trans);
    //             }
    //             if (ImGui::MenuItem("Mesh")) {
    //                 DefaultCube cube;
    //                 ecs.addComponent<DefaultCube>(entity, cube);
    //             }
    //             ImGui::EndMenu();
    //         }
    //
    //         ImGui::Separator();
    //
    //         if (ImGui::MenuItem("Delete Entity")) {
    //             /* TODO logic */
    //         }
    //         ImGui::EndPopup();
    //     }
    //
    //     if (opened) {
    //         ImGui::TreePop();
    //     }
    // }
    //
    // if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
    //     if (ImGui::MenuItem("add new entity")) {
    //         ecs.createEntity();
    //     }
    //     ImGui::EndPopup();
    // }
    // ImGui::End();
}

void ImguiLayer::drawInspector()
{
    // ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiCond_FirstUseEver);
    // ImGui::Begin("Entity Inspector");
    //
    // if (m_selectedEntity == -1) {
    //     ImGui::Text("Select entity to view details");
    //     ImGui::End();
    //     return;
    // }
    //
    // auto& ecs = scene->getEcs();
    //
    // if (ecs.hasComponent<Id>(m_selectedEntity)) {
    //     auto& id = ecs.getComponent<Id>(m_selectedEntity);
    //     char buffer[256];
    //     strncpy(buffer, id.name.c_str(), sizeof(buffer));
    //     if (ImGui::InputText("Name", buffer, sizeof(buffer))) {
    //         id.name = std::string(buffer);
    //     }
    // }
    //
    // ImGui::Separator();
    //
    // if (ecs.hasComponent<Transform>(m_selectedEntity)) {
    //     if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
    //         auto& t = ecs.getComponent<Transform>(m_selectedEntity);
    //         ImGui::DragFloat3("Position", &t.position.x, 0.1f);
    //         ImGui::DragFloat3("Rotation", &t.rotation.x, 0.1f);
    //         ImGui::DragFloat3("Scale", &t.scale.x, 0.1f, 0.1f, 10.0f);
    //     }
    // }
    // ImGui::End();
}

void ImguiLayer::beginWindow(const char* name, bool* open)
{
    // float testSlider = 0.0f;
    // ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
    // ImGui::SetNextWindowSize(ImVec2(300,300), ImGuiCond_FirstUseEver);
    // ImGui::Begin(name, open);
    // ImGui::Text("testing text");
    // ImGui::Checkbox("test checkbox", open);
    // ImGui::SliderFloat("test slider", &testSlider, 0.0f, 1.0f);
}

void ImguiLayer::endWindow()
{
    // ImGui::End();
}
