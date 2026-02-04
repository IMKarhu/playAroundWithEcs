#include "window.h"

// #include "GLFW/glfw3.h"
#include <glad/glad.h>
// #include "imgui.h"
// #include "imgui_impl_glfw.h"
// #include "imgui_impl_opengl3.h"

#include <print>

Window::Window()
    :m_width(600)
    ,m_height(400)
    ,m_title("Ecs testing")
{
    initWindow(m_width, m_height, m_title);
}

Window::Window(uint32_t width, uint32_t height, std::string title)
    :m_width(width)
    ,m_height(height)
    ,m_title(title)
{
    initWindow(m_width, m_height, m_title);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
};

GLFWwindow *Window::handle() const
{
    return m_window;
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

int Window::initWindow(uint32_t width, uint32_t height, std::string title)
{
    int result;
    if(!glfwInit()) {
        std::println("you done fucked up");
        result = -1;
        return result;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if(!m_window) {
        std::println("failed to create window context");
        glfwTerminate();
        result = -1;
        return result;
    }

    glfwMakeContextCurrent(m_window);
    result = gladLoadGL();
    if(result == 1) {
        std::println("success on loading glad");
    }else {
        std::println("failed to load glad");
    }

    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    glViewport(0, 0, w, h);

    glfwSetFramebufferSizeCallback(m_window, framebufferCallback);

    uiExperiment();

    return result;
}

void Window::framebufferCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::uiExperiment()
{
    float main_scale = ImGui_ImplGlfw_GetContentScaleForWindow(m_window);
    //everything is from glfw example
    //https://github.com/ocornut/imgui/blob/master/examples/example_glfw_opengl3/main.cpp
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;
    io.ConfigDpiScaleFonts = true;
    io.ConfigDpiScaleViewports = true;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    const char* glsl_version = "#version 460";
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}
