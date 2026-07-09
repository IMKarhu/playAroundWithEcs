#include "glrenderer.h"
#include "glad/glad.h"
#include "windowing/events.h"
#include "windowing/window.h"
#include "framebuffer.h"
#include "framebufferManager.h"
#include "shader.h"
#include "glshader.h"
#include "glmesh.hpp"
#include "assetManager.h"

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
    :m_window(window)
{
    m_width = m_window.width();
    m_height = m_window.height();
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

    m_graphicsdevice = std::make_unique<Lumos::GLDevice>();

    EventDispatcher::subscribe(EventType::WindowResize, [this](const Event &e) {
            const auto& event = static_cast<const ResizeEvent&>(e);
            m_width = event.width;
            m_height = event.height;
    });
}

GLRenderer::~GLRenderer()
{
    for (auto& [name, ptr] : m_shadercache) {
        delete ptr;
    }
    m_shadercache.clear();
}

void GLRenderer::beginFrame() const
{
}

void GLRenderer::endFrame()
{
}

void GLRenderer::beginPass(const RenderPassDesc& desc, FrameBufferManager& framebuffermanager)
{
    if (desc.framebuffer == "") {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, m_width, m_height);
    }
    else {
        auto framebuffer = framebuffermanager.getFramebuffer(desc.framebuffer);
        framebuffer->bind();
        glViewport(0, 0, framebuffer->framebufferSpec().width, framebuffer->framebufferSpec().height);
    }

    if (desc.depthtest) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }

    glClearColor(desc.clearcolor.x,
                 desc.clearcolor.y,
                 desc.clearcolor.z,
                 desc.clearcolor.w);

    GLbitfield clearmask = 0;
    if (desc.clearcolorbuffer) {
        clearmask |= GL_COLOR_BUFFER_BIT;
    }
    if (desc.cleardepthbuffer) {
        clearmask |= GL_DEPTH_BUFFER_BIT;
    }
    if (clearmask != 0) {
        glClear(clearmask);
    }
}

void GLRenderer::endPass()
{
    // flush();
}

void GLRenderer::submit(RenderInfo info)
{
    m_renderqueue.push_back(info);
}

void GLRenderer::flush(std::function<void()> func, Lumos::AssetManager& assetmanager)
{
    func();
    for(const auto& info : m_renderqueue) {
        auto shaderelement = m_shadercache.find(info.shadername);
        GLShader *shader = static_cast<GLShader*>(shaderelement->second);
        shader->bind();
        switch(info.type)
        {
            case InfoType::Screen:
                glBindTextureUnit(0, info.screenpasscolorattachment.id);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                break;
            case InfoType::Lighting:
                shader->setUniformVec3("u_lightdir", info.lightdir);
                shader->setUniformVec3("u_lightcolor", info.lightcolor);
                glBindTextureUnit(0, info.attachments.attachment0.id);
                glBindTextureUnit(1, info.attachments.attachment1.id);
                glBindTextureUnit(2, info.attachments.attachment2.id);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                break;
            case InfoType::Geometry:
                shader->setUniformMat4("u_model", info.model);
                shader->setUniformMat4("u_viewproj", info.viewproj);
                if (info.mesh) {
                    Lumos::GLMesh* glmesh = static_cast<Lumos::GLMesh*>(info.mesh);

                    for (size_t i = 0; i < glmesh->getSubMeshCount(); i++) {
                        Lumos::RenderPacket packet = glmesh->getSubMeshPacket(i);
                        if (packet.basecolorHandle.isValid()) {
                            auto* tex = assetmanager.getTextureManager().get(packet.basecolorHandle);
                            if (tex) {
                                glBindTextureUnit(0, static_cast<Lumos::GLTexture*>(tex)->rendererID());
                            }
                        }
                        if (packet.normalHandle.isValid()) {
                            auto* tex = assetmanager.getTextureManager().get(packet.normalHandle);
                            if (tex) {
                                glBindTextureUnit(1, static_cast<Lumos::GLTexture*>(tex)->rendererID());
                            }
                        }
                        if (packet.metallicroughnessHandle.isValid()) {
                            auto* tex = assetmanager.getTextureManager().get(packet.metallicroughnessHandle);
                            if (tex) {
                                glBindTextureUnit(2, static_cast<Lumos::GLTexture*>(tex)->rendererID());
                            }
                        }
                        glmesh->prepareSubMesh(i);
                        glDrawElements(GL_TRIANGLES, glmesh->getIndexCount(i), GL_UNSIGNED_INT, 0);
                    }
                }
                break;
            default:
                std::println("info type did not match to any of these: LIGHTING, GEOMETRY, SCREEN");
        }
        shader->unbind();
    }
    m_renderqueue.clear();
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

Lumos::IGraphicsDevice& GLRenderer::getGraphicsDevice()
{
    return *m_graphicsdevice;
}

