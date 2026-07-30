#include "materialManager.h"
#include "string_hash.h"

namespace Lumos
{
    MaterialManager::MaterialManager(IGPUResourceFactory& resourcefactory)
        :m_resourcefactory(resourcefactory) {}

    MaterialHandle MaterialManager::create(std::string_view name, const MaterialResource& resource, TextureManager& texturemanager)
    {
        uint64_t id = StringHash::hash(name);
        if (m_materials.find(id) != m_materials.end()) {
            m_metadata[id].refcount++;
            return MaterialHandle { id };
        }
        GpuMaterial gpumaterial;
        gpumaterial.basecolor = texturemanager.get(resource.basecolor)->bindlessID();
        gpumaterial.normal = texturemanager.get(resource.normal)->bindlessID();
        gpumaterial.metallicroughness = texturemanager.get(resource.metallicroughness)->bindlessID();
        m_gpumaterials.push_back(gpumaterial);

        m_materials[id] = resource;
        m_metadata[id] = AssetRecord{1, "test"};
        return MaterialHandle { id };
    }
    void MaterialManager::createssbobuffer()
    {
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
