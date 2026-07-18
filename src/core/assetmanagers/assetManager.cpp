#include "assetManager.h"

namespace Lumos
{
    AssetManager::AssetManager(IGPUResourceFactory& graphicsdevice)
    {
        m_texturemanager = std::make_unique<TextureManager>(graphicsdevice);
        m_meshmanager = std::make_unique<MeshManager>(graphicsdevice, *m_texturemanager);
    }

    void AssetManager::garbageCollect()
    {
        m_meshmanager->unloadUnused();
        m_texturemanager->unloadUnused();
    }

    TextureManager& AssetManager::getTextureManager()
    {
        return *m_texturemanager;
    }

    MeshManager& AssetManager::getMeshManager()
    {
        return *m_meshmanager;
    }
}//namespace Lumos
