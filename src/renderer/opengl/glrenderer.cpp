#include "glrenderer.h"
#include "glad/glad.h"
#include "windowing/events.h"
#include "windowing/window.h"
#include "framebuffer.h"
#include "framebufferManager.h"
#include "framebufferBuilder.h"
#include "shader.h"
#include "glshader.h"
#include "meshManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

void GLRenderer::renderScene(const RenderInfo &info, const MeshManager& meshmanager) const
{
    auto shaderelement = m_shadercache.find(info.shadername);
    GLShader *shader = static_cast<GLShader*>(shaderelement->second);
    const MeshResource& mesh = meshmanager.get(info.mesh);
    shader->bind();
    shader->setUniformMat4("u_mvp", info.transform);
    if (info.texturehandle > 0) {
        glBindTextureUnit(0, info.texture.id);
        shader->setUniformTexture("ubasecolortexture", 0);
        shader->setUniformInt("uusetexture", true);
    }
    else
    {
        glBindTextureUnit(0, 0);
        shader->setUniformInt("uusetexture", false);
    }
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    shader->unbind();
}

void GLRenderer::endFrame() const
{
    m_framebufferManager->unbind("scene");
}

void GLRenderer::renderToScreen(const RenderInfo &info, const MeshManager& meshmanager) const
{
    auto shaderelement = m_shadercache.find(info.shadername);
    GLShader *shader = static_cast<GLShader*>(shaderelement->second);
    const MeshResource& mesh = meshmanager.get(info.mesh);
    shader->bind();
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, m_framebufferManager->getFramebuffer("scene")->colorAttachment());
    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    shader->unbind();
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

MeshResource GLRenderer::createMeshPrimitive(std::vector<Vertex> vertices,
                                             std::vector<uint32_t> indices) const
{
    MeshResource res;
    res.indexCount = indices.size();
    glCreateVertexArrays(1, &res.vao);
    glCreateBuffers(1, &res.vbo);
    glCreateBuffers(1, &res.ebo);

    glNamedBufferStorage(res.vbo,
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_DYNAMIC_STORAGE_BIT
    );

    glNamedBufferStorage(res.ebo,
            indices.size() * sizeof(uint32_t),
            indices.data(),
            GL_DYNAMIC_STORAGE_BIT
    );


    glVertexArrayVertexBuffer(res.vao, 0, res.vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(res.vao, res.ebo);

    glEnableVertexArrayAttrib(res.vao, 0);
    glVertexArrayAttribFormat(res.vao,
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            offsetof(Vertex, position)
    );
    glVertexArrayAttribBinding(res.vao, 0, 0);

    glEnableVertexArrayAttrib(res.vao, 1);
    glVertexArrayAttribFormat(res.vao,
            1,
            4,
            GL_FLOAT,
            GL_FALSE,
            offsetof(Vertex, color)
    );
    glVertexArrayAttribBinding(res.vao, 1, 0);

    glEnableVertexArrayAttrib(res.vao, 2);
    glVertexArrayAttribFormat(res.vao,
            2,
            4,
            GL_FLOAT,
            GL_FALSE,
            offsetof(Vertex, texcoord)
    );
    glVertexArrayAttribBinding(res.vao, 2, 0);

    return res;
}

TextureHandle GLRenderer::createTexture(const std::string& filepath, TextureImportSettings settings) const
{
    uint32_t handle;
    int x, y, c;
    stbi_uc* pixels = stbi_load(filepath.c_str(), &x, &y, &c, STBI_rgb_alpha);
    if (!pixels) {
        std::println("failed to load texture file");
        handle = -1;
        return { handle };
    }
    glCreateTextures(GL_TEXTURE_2D, 1, &handle);
    glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(handle, 1, GL_RGBA8, x, y);
    glTextureSubImage2D(handle, 0, 0, 0, x, y, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_image_free(pixels);
    return { handle };
}

void GLRenderer::resizeFramebuffer(uint32_t width, uint32_t height)
{
    auto fb = m_framebufferManager->getFramebuffer("scene");
    fb->resize(width, height);
}
