#include "framebufferManager.h"
#include <glad/glad.h>

FrameBufferManager::FrameBufferManager()
{
}

FrameBufferManager::~FrameBufferManager()
{
}

//still need to be fixed to account Vulkan
void FrameBufferManager::renderto(std::shared_ptr<FrameBuffer> framebuffer, std::function<void()> render)
{
    framebuffer->bind();
    glViewport(0, 0, framebuffer->framebufferSpec().width, framebuffer->framebufferSpec().height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render();

    framebuffer->unbind();
}
