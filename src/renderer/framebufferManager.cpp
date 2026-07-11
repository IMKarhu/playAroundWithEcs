#include "framebufferManager.h"
#include <glad/glad.h>
#include <print>

namespace Lumos
{
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
        }
    }

    void FrameBufferManager::unbind(const std::string &name)
    {
        auto it = m_framebuffers.find(name);
        if (it != m_framebuffers.end()) {
            it->second->unbind();
        }
    }

    std::shared_ptr<FrameBuffer> FrameBufferManager::getFramebuffer(const std::string& name)
    {
        auto it = m_framebuffers.find(name);
        return (it != m_framebuffers.end()) ? it->second : nullptr;
    }

    void FrameBufferManager::resizeAll(uint32_t width, uint32_t height)
    {
        for (auto& [name, framebuffer] : m_framebuffers) {
            framebuffer->resize(width, height);
        }
    }
}// namespace Lumos

