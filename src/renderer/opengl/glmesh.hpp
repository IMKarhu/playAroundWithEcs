#pragma once
#include <assetmanagers/assetbase.hpp>
#include <glad/glad.h>

namespace Lumos
{
    struct GLSubMesh
    {
        uint32_t vao = 0;
        uint32_t vbo = 0;
        uint32_t ebo = 0;
        uint32_t indexcount = 0;
        MaterialHandle materialhandle;
    };

    class GLMesh : public IMesh
    {
    public:
        GLMesh(const ModelData& modeldata);
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
        GLTexture(const ImageData& data);
        ~GLTexture() override;

        int width() const override;
        int height() const override;
        uint64_t rendererID() const override;
        uint64_t bindlessID() const override;
    private:
        uint32_t m_rendererID = 0;
        GLuint64 m_bindlesshandle;
        int m_width = 0;
        int m_height = 0;
        int channels = 0;
    };

    class GLResourceFactory : public IGPUResourceFactory
    {
    public:
        std::unique_ptr<ITexture> createTexture(const ImageData& data) override
        {
            return std::make_unique<GLTexture>(data);
        }

        std::unique_ptr<IMesh> createMesh(const ModelData& modeldata) override
        {
            return std::make_unique<GLMesh>(modeldata);
        }
        uint32_t createSSBO(std::vector<GpuMaterial>& materials) override
        {
            uint32_t id;
            glCreateBuffers(1, &id);
            glNamedBufferStorage(id,
                    materials.size() * sizeof(GpuMaterial),
                    materials.data(),
                    GL_DYNAMIC_STORAGE_BIT
            );
            return id;
        }
    };
}//namespace Lumos
