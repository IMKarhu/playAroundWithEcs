#include "glrenderer.h"
#include "glframebuffer.h"
#include "window.h"
#include "components.h"


GLRenderer::GLRenderer(Window& window)
    : m_window(window)
{
    FramebufferSpec spec = {
        .m_width = window.width(),
        .m_height = window.height()
    };
    m_viewportfb = std::make_unique<GLFramebuffer>(spec);
}

void GLRenderer::initialize(std::vector<Entity>& entities)
{
    for(auto entity : entities) {
        if (ECS::hasComponent<Vertices>(entity))
            std::println("has component");

        auto comp = ECS::getComponent<Vertices>(entity);

        glCreateVertexArrays(1, &comp.m_vao);
        glCreateBuffers(1, &comp.m_vbo);
        glCreateBuffers(1, &comp.m_ebo);

        glBindVertexArray(comp.m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, comp.m_vbo);
        glNamedBufferData(comp.m_vao,
                comp.m_vertices.size() * sizeof(Vertex),
                comp.m_vertices.data(),
                GL_STATIC_DRAW
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, comp.m_ebo);
        glNamedBufferData(comp.m_ebo,
                comp.m_indices.size() * sizeof(uint32_t),
                comp.m_indices.data(),
                GL_STATIC_DRAW
        );

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

    }

}

void GLRenderer::beginFrame()
{
    auto& fb = *m_viewportfb;
    fb.bind();
    glViewport(0, 0, fb.framebufferSpec().m_width, fb.framebufferSpec().m_height);
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
