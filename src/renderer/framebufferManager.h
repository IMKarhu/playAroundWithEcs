#pragma once
#include "framebuffer.h"
#include <functional>

class FrameBufferManager
{
public:
    FrameBufferManager();
    ~FrameBufferManager();

    void renderto(std::shared_ptr<FrameBuffer> framebuffer, std::function<void()> render);
private:
};
