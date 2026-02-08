#pragma once
#include "framebuffer.h"

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void beginFrame() = 0;
    virtual void renderScene() = 0;
    virtual void endFrame() = 0;
    virtual FrameBuffer &viewportfb() = 0;
private:
};
