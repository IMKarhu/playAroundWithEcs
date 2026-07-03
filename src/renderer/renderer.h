#pragma once
#include <cstdint>
#include <vector>
#include "renderInfo.h"
#include "ecsImpl/components.h"
#include "textureSettings.h"

class MeshManager;
class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void initialize() const = 0;
    virtual void beginFrame() const = 0;
    virtual void renderScene(const RenderInfo &info, const MeshManager& meshmanager) const = 0;
    virtual void submit(RenderInfo info) = 0;
    virtual void flush(const MeshManager& meshmanager) = 0;
    virtual void endFrame() = 0;
    virtual void renderToScreen(const RenderInfo &info, const MeshManager& meshmanager) const = 0;
    virtual void createAndAddToShaderCache(std::string name,
                                           const std::string vertpath,
                                           const std::string fragpath) = 0;
    virtual MeshResource createMeshPrimitive(std::vector<Vertex> vertices,
                                             std::vector<uint32_t> indices) const = 0;
    virtual TextureHandle createTexture(const std::string& filepath, TextureImportSettings settings) const = 0;
private:
};
