#pragma once
#include <cstdint>
#include <vector>
#include "renderInfo.h"
#include "ecsImpl/components.h"

class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void initialize() const = 0;
    virtual void beginFrame() const = 0;
    virtual void renderScene(const RenderInfo &info) const = 0;
    virtual void endFrame() const = 0;
    virtual void renderToScreen(const RenderInfo &info) const = 0;
    virtual void createAndAddToShaderCache(std::string name,
                                           const std::string vertpath,
                                           const std::string fragpath) = 0;
    virtual void createMeshPrimitive(std::string name,
                                     std::vector<Vertex> vertices,
                                     std::vector<uint32_t> indices,
                                     uint32_t vbo,
                                     uint32_t ebo) const = 0;
private:
};
