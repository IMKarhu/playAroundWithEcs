#include "glrenderer.h"
#include "glad/glad.h"
#include "windowing/events.h"
#include "windowing/window.h"
#include "framebuffer.h"
#include "framebufferManager.h"
#include "framebufferBuilder.h"
#include "shader.h"
#include "glshader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <print>

static void GLAPIENTRY DebugMessageCallback(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam)
{
    std::println("OpenGL Error: type: {} message: {}", type, message);
}


GLRenderer::GLRenderer(const Window &window)
{
    std::println("hello from glrenderer");
    bool result = gladLoadGL();
    if(result == 1) {
        std::println("success on loading glad");
    }else {
        std::println("failed to load glad");
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugMessageCallback, nullptr);
    glDebugMessageControl(
            GL_DONT_CARE,
            GL_DONT_CARE,
            GL_DEBUG_SEVERITY_NOTIFICATION,
            0, nullptr,
            GL_FALSE
    );

    EventDispatcher::subscribe(EventType::WindowResize, [this](const Event &e) {
            const auto& event = static_cast<const ResizeEvent&>(e);
            resizeFramebuffer(event.width, event.height);
    });

    //Demonstrate how events would be used with keypresses
    EventDispatcher::subscribe(EventType::KeyPress, [this](const Event &e) {
            const auto& event = static_cast<const KeyEvent&>(e);
            std::println("pressed a key: {} action: {} mods: {}",event.key,event.action, event.mods);
    });

    m_framebufferManager = std::make_shared<FrameBufferManager>();
    m_framebufferManager->addFramebuffer("scene", FramebufferBuilder()
        .setSize(window.width(),window.height())
        .setSamples(1)
        .setColorAttachments({Format::RGBA8})
        .setDepthFormat(DepthFormat::Depth24Stencil8, DepthFormatType::Texture)
        .build());

}

GLRenderer::~GLRenderer()
{
    for (auto& [name, ptr] : m_shadercache) {
        delete ptr;
    }
    m_shadercache.clear();
}

void GLRenderer::initialize() const
{
}

void GLRenderer::beginFrame() const
{
    m_framebufferManager->bind("scene");
}

void GLRenderer::renderScene(const RenderInfo &info) const
{
    auto shaderelement = m_shadercache.find(info.shadername);
    GLShader *shader = static_cast<GLShader*>(shaderelement->second);
    shader->bind();
    shader->setUniformMat4("u_mvp", info.transform);
    glBindVertexArray(m_vaocache[info.meshname]);
    glDrawElements(GL_TRIANGLES, m_indexcache[info.meshname], GL_UNSIGNED_INT, 0);
    shader->unbind();
}

void GLRenderer::endFrame() const
{
    m_framebufferManager->unbind("scene");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    auto fb = m_framebufferManager->getFramebuffer("scene");
    uint32_t w = fb->framebufferSpec().width;
    uint32_t h = fb->framebufferSpec().height;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fb->colorAttachment());
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitNamedFramebuffer(fb->colorAttachment(), 0, 0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    // glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void GLRenderer::createAndAddToShaderCache(std::string name,
                                           const std::string vertpath,
                                           const std::string fragpath)
{
    std::println("name: {}", name);
    std::println("vertpath: {}", vertpath);
    std::println("fragpath: {}", fragpath);
    m_shadercache[name] = new GLShader(vertpath, fragpath);
}

void GLRenderer::createMeshPrimitive(std::string name,
                                     std::vector<Vertex> vertices,
                                     std::vector<uint32_t> indices,
                                     uint32_t vbo,
                                     uint32_t ebo) const
{
    uint32_t vao;
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(1, &vbo);
    glCreateBuffers(1, &ebo);

    glNamedBufferData(vbo,
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_STATIC_DRAW
    );

    glNamedBufferData(ebo,
            indices.size() * sizeof(uint32_t),
            indices.data(),
            GL_STATIC_DRAW
    );

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(vao, ebo);

    glEnableVertexArrayAttrib(vao, 0);
    glVertexArrayAttribFormat(vao,
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            offsetof(Vertex, position)
    );
    glVertexArrayAttribBinding(vao, 0, 0);

    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao,
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            offsetof(Vertex, color)
    );
    glVertexArrayAttribBinding(vao, 1, 0);

    m_vaocache[name] = vao;
    m_indexcache[name] = indices.size();
}

void GLRenderer::resizeFramebuffer(uint32_t width, uint32_t height)
{
    auto fb = m_framebufferManager->getFramebuffer("scene");
    fb->resize(width, height);
}
