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

class FrameBuffer
{
public:
    FrameBuffer(uint32_t width, uint32_t height)
        : m_width(width)
        , m_height(height)
    {
        create();
    }

    ~FrameBuffer()
    {
    }

    //I am not really a fan of having these bind and unbind functions.. but it works for now..
    void bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    }

    void unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void create()
    {
        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

        // glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
        // glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
        // glTextureStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height);
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::println("Framebuffer is not complete");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint colorAttachment() const { return m_colorAttachment; }

private:
    uint32_t m_width;
    uint32_t m_height;
    GLuint m_fbo;
    GLuint m_rbo;
    GLuint m_colorAttachment;
    GLuint m_depthAttachment;
};


int main()
{
    Window window;
    FrameBuffer framebuffer(window.width(), window.height());
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

    bool demoWindow = true;
    while(!window.shouldClose()) {

        framebuffer.bind();

        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        framebuffer.unbind();

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
        ImGui::Image((void*)framebuffer.colorAttachment(), viewportSize, ImVec2{0,1},ImVec2{1,0});
        ImGui::End();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLFWwindow *backupcontext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        window.makeCtxCurrent(backupcontext);

        window.pollEvents();
        window.swapBuffers();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}
