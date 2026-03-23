#pragma once
#include <cstdint>

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void initialize() = 0;
    virtual void beginFrame() = 0;
    virtual void renderScene(float dt, uint32_t width, uint32_t height) = 0;
    virtual void endFrame() = 0;
private:
};
