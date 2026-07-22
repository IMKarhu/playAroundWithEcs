#include "meshManager.h"
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

        auto modelimportdata = m_importer.import(filepath);
        ModelData modeldata;
        modeldata.submeshes.resize(modelimportdata.submeshes.size());
        for(size_t i = 0; i < modelimportdata.submeshes.size(); i++) {
            modeldata.submeshes[i].vertices = modelimportdata.submeshes[i].vertices;
            modeldata.submeshes[i].indices = modelimportdata.submeshes[i].indices;
        }
        std::vector<AssetHandle> texturehandles;
        for (const auto& tex : modelimportdata.texsources) {
            texturehandles.push_back(m_texturemanager.load(tex));
        }
        resolveMaterials(texturehandles, modelimportdata, modeldata);

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

    void MeshManager::resolveMaterials(std::vector<AssetHandle>& textures, ModelImportData& importdata, ModelData& data)
    {
        for (size_t i = 0; i < importdata.submeshes.size(); i++) {
            data.submeshes[i].materialdata.basecolorHandle = textures[importdata.submeshes[i].indexes.basecolor];
            data.submeshes[i].materialdata.normalHandle = textures[importdata.submeshes[i].indexes.normal];
            data.submeshes[i].materialdata.metallicroughnessHandle = textures[importdata.submeshes[i].indexes.metrough];
        }
    }
}
