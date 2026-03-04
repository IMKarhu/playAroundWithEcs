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
    virtual void renderScene(float dt, uint32_t width, uint32_t height) = 0;
    virtual void endFrame() = 0;
private:
};
