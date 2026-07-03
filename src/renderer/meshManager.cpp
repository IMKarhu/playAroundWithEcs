#include "meshManager.h"
#include "renderer.h"

MeshManager::MeshManager(const Renderer& renderer)
    :m_renderer(renderer)
{
}

MeshManager::~MeshManager() {}

MeshHandle MeshManager::create(std::vector<Vertex>& vertices,
                               std::vector<uint32_t>& indices)
{
    MeshResource res = m_renderer.createMeshPrimitive(vertices,
                                                      indices);
    m_meshes.push_back(std::move(res));
    return MeshHandle {
        static_cast<uint32_t>(m_meshes.size() -1)
    };
}

const MeshResource MeshManager::get(MeshHandle handle) const
{
    return m_meshes[handle.id];
}

