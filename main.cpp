#include "window.h"
#include "ecs.h"
#include <cassert>
#include <cstdio>
#include <array>
#include <print>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

struct tex
{
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
    GLuint m_texID;
};


int main()
{
    Window window;
    tex tex{};
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

    unsigned int indices[] =
    {
        0,1,2,
        2,3,0
    };

    float vertices[] =
    {
        -1.0f,-1.0f, 0.0f,  0.0f, 0.0f,
         1.0f,-1.0f, 0.0f,  1.0f, 0.0f,
         1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f,  0.0f, 1.0f
    };
    glCreateVertexArrays(1, &tex.m_vao);
    glCreateBuffers(1, &tex.m_vbo);
    glCreateBuffers(1, &tex.m_ebo);
    glNamedBufferData(tex.m_vbo, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glNamedBufferData(tex.m_ebo, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexArrayAttrib(tex.m_vao, 0);
    glVertexArrayAttribBinding(tex.m_vao, 0, 0);
    glVertexArrayAttribFormat(tex.m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glEnableVertexArrayAttrib(tex.m_vao, 1);
    glVertexArrayAttribBinding(tex.m_vao, 1, 0);
    glVertexArrayAttribFormat(tex.m_vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
    glVertexArrayVertexBuffer(tex.m_vao, 0, tex.m_vbo, 0, 5 * sizeof(GLfloat));
    glVertexArrayElementBuffer(tex.m_vao, tex.m_ebo);
    glCreateTextures(GL_TEXTURE_2D, 1, &tex.m_texID);
    glTextureParameteri(tex.m_texID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(tex.m_texID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(tex.m_texID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex.m_texID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(tex.m_texID, 1, GL_RGBA32F, window.width(), window.height());
    glBindImageTexture(0, tex.m_texID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    bool demoWindow = true;
    while(!window.shouldClose()) {
        window.pollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(&demoWindow);
        /*DockSpace*/
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
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
        /*viewport*/
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Viewport", &demoWindow, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::PopStyleVar(2);
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        ImGui::Image((void*)tex.m_texID, viewportSize, ImVec2{0,1},ImVec2{1,0});
        ImGui::End();

        ImGui::Render();

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLFWwindow *backupcontext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        window.makeCtxCurrent(backupcontext);

        window.swapBuffers();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
