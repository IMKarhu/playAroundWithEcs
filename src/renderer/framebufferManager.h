#pragma once
#include "framebuffer.h"
#include <unordered_map>
#include <string>
#include <memory>

#ifdef _WIN32
    #ifdef RENDERER_EXPORT
    #define RENDERER_API __declspec(dllexport)
    #else
    #define RENDERER_API __declspec(dllimport)
    #endif
#else
    #define RENDERER_API
#endif

class RENDERER_API FrameBufferManager
{
public:
    FrameBufferManager();
    ~FrameBufferManager();

    void addFramebuffer(const std::string &name, std::shared_ptr<FrameBuffer> framebuffer);
    void bind(const std::string &name);
    void unbind(const std::string &name);
    std::shared_ptr<FrameBuffer> getFramebuffer(const std::string& name);
    void resizeAll(uint32_t width, uint32_t height);
private:
    std::unordered_map<std::string, std::shared_ptr<FrameBuffer>> m_framebuffers;
};
