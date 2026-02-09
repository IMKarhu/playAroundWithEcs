#pragma once
#include "framebuffer.h"
#include "ecs.h"
#include <vector>

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void initialize(std::vector<Entity>& entities) = 0;
    virtual void beginFrame() = 0;
    virtual void renderScene() = 0;
    virtual void endFrame() = 0;
    virtual FrameBuffer &viewportfb() = 0;
private:
};
