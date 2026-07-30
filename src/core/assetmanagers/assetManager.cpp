#include "assetManager.h"

namespace Lumos
{
    AssetManager::AssetManager(IGPUResourceFactory& graphicsfactory)
    {
        m_texturemanager = std::make_unique<TextureManager>(graphicsfactory);
        m_materialmanager = std::make_unique<MaterialManager>(graphicsfactory);
        m_meshmanager = std::make_unique<MeshManager>(graphicsfactory, *m_texturemanager, *m_materialmanager);
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

    MaterialManager& AssetManager::getMaterialManager()
    {
        return *m_materialmanager;
    }
}//namespace Lumos
