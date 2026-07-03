#pragma once
#include <unordered_map>
#include <string_view>
#include <vector>

#include "ecsImpl/components.h"

class Renderer;

class MeshManager
{
public:
    MeshManager(const Renderer& renderer);
    ~MeshManager();

    MeshHandle create(std::vector<Vertex>& vertices,
                      std::vector<uint32_t>& indices);
    const MeshResource get(MeshHandle handle) const;
private:
    std::vector<MeshResource> m_meshes;
    const Renderer& m_renderer;
};
