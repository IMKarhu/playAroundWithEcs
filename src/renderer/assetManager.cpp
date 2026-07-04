#include "assetManager.h"
#include <filesystem>
#include <print>

#include "glad/glad.h"
// #include "modelImporter.h"
#include "renderer.h"
#include "textureSettings.h"


bool isGltfFile(std::string_view file)
{
    return file.ends_with(".gltf");
}

AssetManager::AssetManager(const Renderer& renderer)
    :m_renderer(renderer)
{
    m_texturemanager = std::make_unique<TextureManager>(renderer);
    m_meshmanager = std::make_unique<MeshManager>(renderer);
    std::string name = "screen";
    ModelAsset asset;
    ModelPrimitive primitive;
    ScreenQuad quad = ModelImporter::screenQuad();
    primitive.mesh = m_meshmanager->create(quad.vertices, quad.indices);
    asset.handles.push_back(std::move(primitive));
    m_assetpool.emplace(name, asset);

}

AssetManager::~AssetManager() {}


void AssetManager::loadAssetsFromDirectory(std::string_view dirpath)
{
    bool exists = std::filesystem::exists(dirpath);
    if (exists) {
        for (const auto& entry : std::filesystem::directory_iterator(dirpath)) {
            if (!entry.is_regular_file() || !isGltfFile(entry.path().filename().string())) {
                continue;
            }
            loadModel(entry.path());
        }
    }
}

const ModelAsset& AssetManager::getModelAsset(const std::string& id) const
{
    auto it = m_assetpool.find(id);
    assert(it != m_assetpool.end() && "ID could not be found from assetpool");

    return it->second;
}

void AssetManager::loadModel(std::filesystem::path path)
{
    std::string assetname = path.filename().string();
    std::println("file name: {}", assetname);
    ModelImporter::ModelImportData data = ModelImporter::importDataFromFile(m_assetpath.string()+assetname);
    data.outMesh.name = assetname;
    
    loadTextures(data);
    createMeshes(data);
}

void AssetManager::loadTextures(ModelImporter::ModelImportData data)
{
    for (const auto& tex : data.textures) {
        std::println("texture path: {}", m_assetpath.string()+tex.filename);
        std::string texfullpath = m_assetpath.string()+tex.filename;
        TextureImportSettings settings;
        TextureHandle handle = m_texturemanager->load(texfullpath, settings);
        m_localtexturehandles.push_back(handle);
    }
}

void AssetManager::createMeshes(ModelImporter::ModelImportData data)
{
    ModelAsset asset;
    for (size_t i = 0; i < data.outMesh.submeshes.size(); i++) {
        std::println("subdata.outMesh name: {}", data.outMesh.submeshes[i].name);
        ModelPrimitive primitive;
        primitive.mesh = m_meshmanager->create(data.outMesh.submeshes[i].vertices,
                                                  data.outMesh.submeshes[i].indices);
        uint32_t matindex = data.submeshmaterialindex[i];
        if (matindex >= 0 && matindex < data.materialhandles.size()) {
            const auto& material = data.materialhandles[matindex];
            auto resolve = [&](int32_t idx) -> TextureHandle {
                if (idx >= 0 && idx < static_cast<int32_t>(m_localtexturehandles.size())) {
                    return m_localtexturehandles[idx];
                }
                return {};
            };
            primitive.basecolorHandle = resolve(material.basecolorhandle);
            primitive.normalHandle = resolve(material.normalhandle);
            primitive.metallicroughnessHandle = resolve(material.metallicroughnesshandle);
        }
        asset.handles.push_back(std::move(primitive));
    }
    m_localtexturehandles.clear();
    m_assetpool.emplace(data.outMesh.name, asset);
}

MeshManager* AssetManager::getMeshManager() const
{
    return m_meshmanager.get();
}

