#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace Lumos
{
    struct AssetHandle
    {
        uint64_t id = 0;
        bool isValid() const
        {
            return id != 0;
        }
        bool operator==(const AssetHandle& other) const
        {
            return id == other.id;
        }
    };

    struct GBufferHandles
    {
        AssetHandle attachment0;
        AssetHandle attachment1;
        AssetHandle attachment2;
    };

    //internal metadata for managers
    struct AssetRecord
    {
        uint32_t refcount = 0;
        std::string filepath;
    };

    class ISubAssetManager
    {
    public:
        virtual ~ISubAssetManager() = default;
        virtual void unloadUnused() = 0;
    };

    class IAsset
    {
    public:
        virtual ~IAsset() = default;
    };

    class ITexture : public IAsset
    {
    public:
        ~ITexture() override = default;
        virtual int width() const = 0;
        virtual int height() const = 0;
        virtual uint64_t rendererID() const = 0;
    };

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 texcoords;
        glm::vec4 tangents;
    };

    struct SubMeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        AssetHandle basecolorHandle;
        AssetHandle normalHandle;
        AssetHandle metallicroughnessHandle;
    };

    struct RenderPacket
    {
        AssetHandle basecolorHandle;
        AssetHandle normalHandle;
        AssetHandle metallicroughnessHandle;

        glm::vec3 basecolorfactor = glm::vec3(1);
        glm::vec3 metallicroughnessfactor = glm::vec3(1);
    };

    class IMesh : public IAsset
    {
    public:
        ~IMesh() override = default;
        virtual void bind(size_t index) = 0;
        virtual size_t getSubMeshCount() const = 0;
        virtual void prepareSubMesh(size_t index) = 0;
        virtual RenderPacket getSubMeshPacket(size_t index) = 0;
    };
    class IGraphicsDevice
    {
    public:
        virtual ~IGraphicsDevice() = default;
        virtual std::unique_ptr<ITexture> createTexture(const std::string& path) = 0;
        virtual std::unique_ptr<IMesh> createMesh(const std::vector<SubMeshData>& submeshes) = 0;
    };

} //namespace Lumos
