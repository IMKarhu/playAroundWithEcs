#pragma once
#include <cstdint>
#include <string>
#include <memory>
#include <vector>
#include <variant>
#include <filesystem>
#include <span>
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

    enum MaterialFlags : uint32_t
    {
        MaterialNone = 0,
        MaterialBasecolorMap = 1 << 0,
        MaterialNormalMap = 1 << 1,
        MaterialMetalroughMap = 1 << 2
    };

    struct MaterialHandle
    {
        uint64_t id = 0;
        bool isValid() const
        {
            return id != 0;
        }
        bool operator==(const MaterialHandle& other) const
        {
            return id == other.id;
        }
    };

    struct MaterialResource
    {
        AssetHandle basecolor;
        AssetHandle normal;
        AssetHandle metallicroughness;

        glm::vec3 basecolorfactor = glm::vec3(1.0f);
        float metallicfactor = 1.0f;
        float roughnessfactor = 1.0f;
        uint32_t flags = MaterialNone;
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
        glm::vec2 texcoords;
        glm::vec4 tangents;
    };
    struct MaterialData
    {
        AssetHandle basecolorHandle;
        AssetHandle normalHandle;
        AssetHandle metallicroughnessHandle;
    };

    struct SubMeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        MaterialHandle materialhandle;
    };

    struct TextureSource
    {
        enum class Type
        {
            File,
            Byte
        };
        Type type;
        std::string cachekey;
        std::filesystem::path path;
        std::span<const uint8_t> bytes;
        std::string mimetype;
    };

    struct ModelData
    {
        std::vector<SubMeshData> submeshes;
        std::vector<TextureSource> texturesources;
    };

    struct RenderPacket
    {
        MaterialHandle handle;
    };

    struct ImageData
    {
        int width;
        int height;
        int channels;
        std::vector<uint8_t> pixels;
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
    class IGPUResourceFactory
    {
    public:
        virtual ~IGPUResourceFactory() = default;
        virtual std::unique_ptr<ITexture> createTexture(const ImageData& data) = 0;
        virtual std::unique_ptr<IMesh> createMesh(const ModelData& modeldata) = 0;
    };

} //namespace Lumos
