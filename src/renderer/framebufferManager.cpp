#include "framebufferManager.h"
#include <glad/glad.h>
#include <print>

FrameBufferManager::FrameBufferManager()
{
}

FrameBufferManager::~FrameBufferManager()
{
}

void FrameBufferManager::addFramebuffer(const std::string &name, std::shared_ptr<FrameBuffer> framebuffer)
{
    m_framebuffers[name] = framebuffer;
}

void FrameBufferManager::bind(const std::string &name)
{
    auto it = m_framebuffers.find(name);
    if (it != m_framebuffers.end()) {
        it->second->bind();
        glViewport(0, 0, it->second->framebufferSpec().width, it->second->framebufferSpec().height);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void FrameBufferManager::unbind(const std::string &name)
{
    auto it = m_framebuffers.find(name);
    if (it != m_framebuffers.end()) {
        it->second->unbind();
    }
}

std::shared_ptr<FrameBuffer> FrameBufferManager::getFramebuffer(const std::string &name)
{
    auto it = m_framebuffers.find(name);
    return (it != m_framebuffers.end()) ? it->second : nullptr;
}
