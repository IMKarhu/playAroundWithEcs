#include "glrenderer.h"
#include "glframebuffer.h"
#include "window.h"


GLRenderer::GLRenderer(Window& window)
    : m_window(window)
{
    FramebufferSpec spec = {
        .m_width = window.width(),
        .m_height = window.height()
    };
    m_viewportfb = std::make_unique<GLFramebuffer>(spec);
}

void GLRenderer::beginFrame()
{
    auto& fb = *m_viewportfb;
    fb.bind();
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void GLRenderer::renderScene()
{
}

void GLRenderer::endFrame()
{
    auto& fb = *m_viewportfb;
    fb.unbind();
}

FrameBuffer &GLRenderer::viewportfb()
{
    return *m_viewportfb;
}
