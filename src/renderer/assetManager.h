#pragma once
#include <string_view>
#include <filesystem>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "ecsImpl/components.h"
#include "modelImporter.h"
#include "textureManager.h"
#include "meshManager.h"

#ifdef _WIN32
    #ifdef RENDERER_EXPORT
    #define RENDERER_API __declspec(dllexport)
    #else
    #define RENDERER_API __declspec(dllimport)
    #endif
#else
    #define RENDERER_API
#endif

class Renderer;

struct ModelPrimitive
{
    MeshHandle mesh;
    TextureHandle basecolorHandle;
    TextureHandle normalHandle;
    TextureHandle metallicroughnessHandle;
};

struct ModelAsset
{
    std::vector<ModelPrimitive> handles;
};

class RENDERER_API AssetManager
{
public:
    AssetManager(const Renderer& renderer);
    ~AssetManager();

    void loadAssetsFromDirectory(std::string_view dirpath);
    const ModelAsset& getModelAsset(const std::string& id) const;
    MeshManager* getMeshManager() const;
private:
    std::filesystem::path m_assetpath = "../src/game/assets/";
    std::unordered_map<std::string, ModelAsset> m_assetpool;
    const Renderer& m_renderer;
    std::unique_ptr<TextureManager> m_texturemanager;
    std::unique_ptr<MeshManager> m_meshmanager;

    void loadModel(std::filesystem::path path);
    void loadTextures(ModelImporter::ModelImportData data);
    void createMeshes(ModelImporter::ModelImportData data);
    void buildMaterials();

    std::vector<TextureHandle> m_localtexturehandles;
};
