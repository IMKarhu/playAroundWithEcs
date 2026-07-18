#include "textureManager.h"
#include "string_hash.h"

namespace Lumos
{
    TextureManager::TextureManager(IGPUResourceFactory& graphicsdevice)
        :m_graphicsdevice(graphicsdevice)
    {
    }

    AssetHandle TextureManager::load(const std::string& filepath)
    {
        uint64_t id = StringHash::hash(filepath);

        if (m_textures.find(id) != m_textures.end()) {
            m_metadata[id].refcount++;
            return AssetHandle { id };
        }

        m_textures[id] = m_graphicsdevice.createTexture(filepath);
        m_metadata[id] = AssetRecord{ 1, filepath };

        return AssetHandle{ id };
    }

    ITexture* TextureManager::get(AssetHandle handle)
    {
        auto it = m_textures.find(handle.id);
        if (it != m_textures.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void TextureManager::incrementRefCount(AssetHandle handle)
    {
        if (m_metadata.find(handle.id) != m_metadata.end()) {
            m_metadata[handle.id].refcount++;
        }
    }

    void TextureManager::decrementRefCount(AssetHandle handle)
    {
        auto it = m_metadata.find(handle.id);
        if (it != m_metadata.end() && it->second.refcount > 0) {
            it->second.refcount--;
        }
    }

    void TextureManager::unloadUnused()
    {
        for (auto it = m_metadata.begin(); it != m_metadata.end();) {
            if (it->second.refcount == 0) {
                m_textures.erase(it->first);
                it = m_metadata.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}//namespace Lumos
