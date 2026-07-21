#pragma once
#include "assetbase.hpp"
#include "textureManager.h"
#include "tiny_gltf_v3.h"
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
        MeshManager(IGPUResourceFactory& resourcefactory, TextureManager& texturemanager);

        AssetHandle load(const std::string& filepath);
        IMesh* get(AssetHandle handle);
        void unloadUnused() override;
    private:
        std::unordered_map<uint64_t, std::shared_ptr<IMesh>> m_meshes;
        std::unordered_map<uint64_t, AssetRecord> m_metadata;
        TextureManager& m_texturemanager;
        IGPUResourceFactory& m_resourcefactory;

        tg3_model parsemodel(const std::string& path, tg3_error_stack& errors);
        std::vector<AssetHandle> loadTextures(const tg3_model& model);
        std::vector<SubMeshData> buildSubMeshes(const tg3_model& model, const std::vector<AssetHandle>& textures);
    };
}//namespace Lumos

