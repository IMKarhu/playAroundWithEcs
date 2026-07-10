#pragma once
#include <assetmanagers/assetbase.h>
#include <glad/glad.h>

namespace Lumos
{
    struct GLSubMesh
    {
        uint32_t vao = 0;
        uint32_t vbo = 0;
        uint32_t ebo = 0;
        uint32_t indexcount = 0;
        AssetHandle basecolorhandle;
        AssetHandle normalhandle;
        AssetHandle metallicroughnesshandle;
        glm::vec3 basecolorfactor = glm::vec3(1);
        glm::vec3 metallicroughnessfactor = glm::vec3(1);
    };

    class GLMesh : public IMesh
    {
    public:
        GLMesh(const std::vector<SubMeshData>& cpudata);
        ~GLMesh() override;
        void bind(size_t index) override;
        size_t getSubMeshCount() const override;
        void prepareSubMesh(size_t index) override;
        RenderPacket getSubMeshPacket(size_t index) override;
        uint32_t getIndexCount(size_t index);
        const std::vector<GLSubMesh>& getSubMeshes() const;
    private:
        std::vector<GLSubMesh> m_submeshes;
    };

    class GLTexture : public ITexture
    {
    public:
        GLTexture(const std::string& filepath);
        ~GLTexture() override;

        int width() const override;
        int height() const override;
        uint64_t rendererID() const override;
    private:
        uint32_t m_rendererID = 0;
        int m_width = 0;
        int m_height = 0;
        int channels = 0;
    };

    class GLDevice : public IGraphicsDevice
    {
    public:
        std::unique_ptr<ITexture> createTexture(const std::string& path) override
        {
            return std::make_unique<GLTexture>(path);
        }

        std::unique_ptr<IMesh> createMesh(const std::vector<SubMeshData>& submeshes) override
        {
            return std::make_unique<GLMesh>(submeshes);
        }
    };
}//namespace Lumos
