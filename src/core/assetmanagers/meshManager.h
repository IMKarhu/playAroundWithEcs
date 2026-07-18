#pragma once
#include "assetbase.h"
#include "textureManager.h"
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
        MeshManager(IGraphicsDevice& graphicsdevice, TextureManager& texturemanager);

        AssetHandle load(const std::string& filepath);
        AssetHandle loadQuad();
        IMesh* get(AssetHandle handle);
        void incrementRefCount(AssetHandle handle);
        void decrementRefCount(AssetHandle handle);
        void unloadUnused() override;
    private:
        std::unordered_map<uint64_t, std::shared_ptr<IMesh>> m_meshes;
        std::unordered_map<uint64_t, AssetRecord> m_metadata;
        TextureManager& m_texturemanager;
        IGraphicsDevice& m_graphicsdevice;
    };
}//namespace Lumos

