#include "textureManager.h"
#include "string_hash.h"
#include "imageDecoder.h"

namespace Lumos
{
    TextureManager::TextureManager(IGPUResourceFactory& resourcefactory)
        :m_resourcefactory(resourcefactory)
    {
    }

    AssetHandle TextureManager::load(const TextureSource& src)
    {
        uint64_t id = StringHash::hash(src.cachekey);

        if (m_textures.find(id) != m_textures.end()) {
            m_metadata[id].refcount++;
            return AssetHandle { id };
        }

        ImageDecoder decoder;
        ImageData imgdata = decoder.decode(src);

        m_textures[id] = m_resourcefactory.createTexture(imgdata);
        m_metadata[id] = AssetRecord{ 1, src.path.string() };

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
