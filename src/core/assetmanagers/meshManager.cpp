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

    MeshManager::MeshManager(IGPUResourceFactory& resourcefactory, TextureManager& texturemanager, MaterialManager& materialmanager)
        :m_texturemanager(texturemanager)
        ,m_materialmanager(materialmanager)
        ,m_resourcefactory(resourcefactory) {}

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

    AssetHandle MeshManager::load(const ModelData& data)
    {
        std::string test = "testmap";
        uint64_t id = StringHash::hash(test);
        if (m_meshes.find(id) != m_meshes.end()) {
            m_metadata[id].refcount++;
            return AssetHandle{ id };
        }
        m_meshes[id] = m_resourcefactory.createMesh(data);
        m_metadata[id] = AssetRecord{ 1, test};
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
        std::string name = "material";
        for (size_t i = 0; i < importdata.submeshes.size(); i++) {
            MaterialResource resource;
            if (importdata.submeshes[i].indexes.basecolor >= 0) {
                resource.basecolor = textures[importdata.submeshes[i].indexes.basecolor];
            }
            if (importdata.submeshes[i].indexes.normal >= 0) {
                resource.normal = textures[importdata.submeshes[i].indexes.normal];
            }
            if (importdata.submeshes[i].indexes.metrough >= 0) {
                resource.metallicroughness = textures[importdata.submeshes[i].indexes.metrough];
            }
            resource.basecolorfactor = importdata.submeshes[i].indexes.basecolorfactor;
            resource.metallicfactor = importdata.submeshes[i].indexes.metallicfactor;
            resource.roughnessfactor = importdata.submeshes[i].indexes.roughnessfactor;
            data.submeshes[i].materialhandle = m_materialmanager.create(name+std::to_string(i), resource, m_texturemanager);
        }
        m_materialmanager.createssbobuffer();
    }
}
