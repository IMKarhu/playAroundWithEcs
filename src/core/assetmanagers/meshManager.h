#pragma once
#include "assetbase.hpp"
#include "textureManager.h"
#include "materialManager.h"
#include "modelImporter.h"
#include <vector>

#ifdef _WIN32
    #ifdef CORE_EXPORT
    #define CORE_API __declspec(dllexport)
    #else
    #define CORE_API __declspec(dllimport)
    #endif
#else
    #define CORE_API
#endif


namespace Lumos
{
    class CORE_API MeshManager : public ISubAssetManager
    {
    public:
        MeshManager(IGPUResourceFactory& resourcefactory, TextureManager& texturemanager, MaterialManager& materialmanager);

        AssetHandle load(const std::string& filepath);
        AssetHandle load(const ModelData& data);
        IMesh* get(AssetHandle handle);
        void unloadUnused() override;
    private:
        std::unordered_map<uint64_t, std::shared_ptr<IMesh>> m_meshes;
        std::unordered_map<uint64_t, AssetRecord> m_metadata;
        TextureManager& m_texturemanager;
        MaterialManager& m_materialmanager;
        IGPUResourceFactory& m_resourcefactory;
        ModelImporter m_importer;

        void resolveMaterials(std::vector<AssetHandle>& textures, ModelImportData& importdata, ModelData& data);
    };
}//namespace Lumos

