#pragma once
#include <cstdint>
#include <vector>
#include "renderInfo.h"
#include "ecsImpl/components.h"
#include "textureSettings.h"
#include "renderPassDesc.h"

class MeshManager;
class FrameBufferManager;
class Renderer
{
public:
    virtual ~Renderer() = default;

    virtual void beginFrame() const = 0;
    virtual void endFrame() = 0;

    virtual void beginPass(const RenderPassDesc& desc, FrameBufferManager& framebuffermanager) = 0;
    virtual void endPass(RenderPassDesc desc) = 0;

    virtual void submit(RenderInfo info) = 0;
    virtual void flush(const MeshManager& meshmanager) = 0;
    virtual void createAndAddToShaderCache(std::string name,
                                           const std::string vertpath,
                                           const std::string fragpath) = 0;
    virtual MeshResource createMeshPrimitive(std::vector<Vertex> vertices,
                                             std::vector<uint32_t> indices) const = 0;
    virtual TextureHandle createTexture(const std::string& filepath, TextureImportSettings settings) const = 0;
private:
};
