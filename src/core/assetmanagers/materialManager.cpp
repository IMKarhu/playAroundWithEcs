#include "materialManager.h"
#include "string_hash.h"
#include <print>

namespace Lumos
{
    MaterialManager::MaterialManager(IGPUResourceFactory& resourcefactory)
        :m_resourcefactory(resourcefactory) {}

    MaterialHandle MaterialManager::create(std::string_view name, MaterialResource& resource, TextureManager& texturemanager)
    {
        uint64_t id = StringHash::hash(name);
        if (m_materials.find(id) != m_materials.end()) {
            m_metadata[id].refcount++;
            return MaterialHandle { id };
        }
        resource.index = static_cast<uint32_t>(m_gpumaterials.size());
        std::println("material idx: {}", resource.index);
        m_materials[id] = resource;
        m_metadata[id] = AssetRecord{1, "test"};

        GpuMaterial gpumaterial;
        gpumaterial.basecolor = texturemanager.get(resource.basecolor)->bindlessID();
        gpumaterial.normal = texturemanager.get(resource.normal)->bindlessID();
        gpumaterial.metallicroughness = texturemanager.get(resource.metallicroughness)->bindlessID();
        
        m_gpumaterials.push_back(gpumaterial);

        return MaterialHandle { id };
    }
    void MaterialManager::createssbobuffer()
    {
        std::println("size of gpumaterials: {}", m_gpumaterials.size());
        m_materialssbo = m_resourcefactory.createSSBO(m_gpumaterials);
    }

    MaterialResource* MaterialManager::get(MaterialHandle handle)
    {
        auto it = m_materials.find(handle.id);
        if (it != m_materials.end()) {
            return &it->second;
        }
        return nullptr;
    }

    const MaterialResource* MaterialManager::get(MaterialHandle handle) const
    {
        auto it = m_materials.find(handle.id);
        if (it != m_materials.end()) {
            return &it->second;
        }
        return nullptr;
    }

    uint32_t MaterialManager::ssboHandle()
    {
        return m_materialssbo;
    }

    void MaterialManager::unloadUnused()
    {
        for (auto it = m_metadata.begin(); it != m_metadata.end(); ++it) {
            if (it->second.refcount == 0) {
                m_materials.erase(it->first);
                it = m_metadata.erase(it);
            }
        }
    }
}
