#include "glmesh.hpp"
#include <print>

namespace Lumos
{
    GLMesh::GLMesh(const ModelData& modeldata)
    {
        for (const auto& data : modeldata.submeshes) {
            GLSubMesh mesh;
            mesh.indexcount = static_cast<uint32_t>(data.indices.size());
            mesh.materialhandle = data.materialhandle;
            glCreateVertexArrays(1, &mesh.vao);
            glCreateBuffers(1, &mesh.vbo);
            glCreateBuffers(1, &mesh.ebo);

            glNamedBufferStorage(mesh.vbo,
                    data.vertices.size() * sizeof(Vertex),
                    data.vertices.data(),
                    GL_DYNAMIC_STORAGE_BIT
            );

            glNamedBufferStorage(mesh.ebo,
                    data.indices.size() * sizeof(uint32_t),
                    data.indices.data(),
                    GL_DYNAMIC_STORAGE_BIT
            );


            glVertexArrayVertexBuffer(mesh.vao, 0, mesh.vbo, 0, sizeof(Vertex));
            glVertexArrayElementBuffer(mesh.vao, mesh.ebo);

            glEnableVertexArrayAttrib(mesh.vao, 0);
            glVertexArrayAttribFormat(mesh.vao,
                    0,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    offsetof(Vertex, position)
            );
            glVertexArrayAttribBinding(mesh.vao, 0, 0);

            glEnableVertexArrayAttrib(mesh.vao, 1);
            glVertexArrayAttribFormat(mesh.vao,
                    1,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    offsetof(Vertex, normal)
            );
            glVertexArrayAttribBinding(mesh.vao, 1, 0);

            glEnableVertexArrayAttrib(mesh.vao, 2);
            glVertexArrayAttribFormat(mesh.vao,
                    2,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    offsetof(Vertex, texcoords)
            );
            glVertexArrayAttribBinding(mesh.vao, 2, 0);

            glEnableVertexArrayAttrib(mesh.vao, 3);
            glVertexArrayAttribFormat(mesh.vao,
                    3,
                    4,
                    GL_FLOAT,
                    GL_FALSE,
                    offsetof(Vertex, tangents)
            );
            glVertexArrayAttribBinding(mesh.vao, 3, 0);

            m_submeshes.push_back(mesh);
        }
    }

    GLMesh::~GLMesh()
    {
        for (const auto& submesh : m_submeshes) {
            glDeleteVertexArrays(1, &submesh.vao);
            glDeleteBuffers(1, &submesh.vbo);
            glDeleteBuffers(1, &submesh.ebo);
        }
    }

    void GLMesh::bind(size_t index)
    {
        glBindVertexArray(m_submeshes[index].vao);
    }

    size_t GLMesh::getSubMeshCount() const
    {
        return m_submeshes.size();
    }

    void GLMesh::prepareSubMesh(size_t index)
    {
        if (index < m_submeshes.size()) {
            glBindVertexArray(m_submeshes[index].vao);
        }
    }

    RenderPacket GLMesh::getSubMeshPacket(size_t index)
    {
        RenderPacket packet;
        if (index < m_submeshes.size()) {
            const auto& sub = m_submeshes[index];
            packet.handle = sub.materialhandle;
        }
        return packet;
    }

    uint32_t GLMesh::getIndexCount(size_t index)
    {
        return m_submeshes[index].indexcount;
    }

    const std::vector<GLSubMesh>& GLMesh::getSubMeshes() const
    {
        return m_submeshes;
    }


    /*TEXTURE*/

    GLTexture::GLTexture(const ImageData& data)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureStorage2D(m_rendererID, 1, GL_RGBA8, data.width, data.height);
        glTextureSubImage2D(m_rendererID, 0, 0, 0, data.width, data.height, GL_RGBA, GL_UNSIGNED_BYTE, data.pixels.data());

        if (data.bindless) {
            m_bindlesshandle = glGetTextureHandleARB(m_rendererID);
            glMakeTextureHandleResidentARB(m_bindlesshandle);
            std::println("bindless texture handle: {}", m_bindlesshandle);
        }
    }

    GLTexture::~GLTexture()
    {
        glMakeTextureHandleNonResidentARB(m_bindlesshandle);
        glDeleteTextures(1, &m_rendererID);
    }

    int GLTexture::width() const
    {
        return m_width;
    }

    int GLTexture::height() const
    {
        return m_height;
    }

    uint64_t GLTexture::rendererID() const
    {
        return static_cast<uint64_t>(m_rendererID);
    }

    uint64_t GLTexture::bindlessID() const
    {
        return m_bindlesshandle;
    }

}//namespace Lumos
