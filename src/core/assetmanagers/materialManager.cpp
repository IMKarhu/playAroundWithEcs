#include "materialManager.h"
#include "string_hash.h"

namespace Lumos
{
    MaterialManager::MaterialManager() {}

    MaterialHandle MaterialManager::create(std::string_view name, const MaterialResource& resource)
    {
        uint64_t id = StringHash::hash(name);
        if (m_materials.find(id) != m_materials.end()) {
            m_metadata[id].refcount++;
            return MaterialHandle { id };
        }
        m_materials[id] = resource;
        m_metadata[id] = AssetRecord{1, "test"};
        return MaterialHandle { id };
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
