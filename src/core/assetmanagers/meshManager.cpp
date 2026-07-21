#include "meshManager.h"
#include "ModelImporter.h"
#include "string_hash.h"

#include <print>


namespace Lumos
{
    bool isGltfFile(std::string_view file) {
        return file.ends_with(".gltf");
    }

    bool isGlbFile(std::string_view file) {
        return file.ends_with(".glb");
    }

    MeshManager::MeshManager(IGPUResourceFactory& resourcefactory, TextureManager& texturemanager)
        :m_texturemanager(texturemanager)
        ,m_resourcefactory(resourcefactory)
    {
    }

    AssetHandle MeshManager::load(const std::string& filepath)
    {
        uint64_t id = StringHash::hash(filepath);

        if (m_meshes.find(id) != m_meshes.end()) {
            m_metadata[id].refcount++;
            return AssetHandle{ id };
        }

        ModelImporter importer;

        auto modeldata = importer.import(filepath);
        std::vector<AssetHandle> texturehandles;
        auto textures = importer.parseImages();
        for (const auto& tex : textures) {
            texturehandles.push_back(m_texturemanager.load(tex.path));
        }
        importer.parseMaterials(modeldata, texturehandles);
        importer.destroyctx();

        m_meshes[id] = m_resourcefactory.createMesh(modeldata);
        m_metadata[id] = AssetRecord{ 1, filepath };
        return AssetHandle { id };
    }

    IMesh* MeshManager::get(AssetHandle handle)
    {
        auto it = m_meshes.find(handle.id);
        if (it != m_meshes.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    void MeshManager::unloadUnused()
    {
        for (auto it = m_metadata.begin(); it != m_metadata.end();) {
            if (it->second.refcount == 0) {
                m_meshes.erase(it->first);
                it = m_metadata.erase(it);
            }
            else {
                ++it;
            }
        }

    }
}
