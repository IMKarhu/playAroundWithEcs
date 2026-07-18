#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include "assetbase.h"

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
    class CORE_API TextureManager : public ISubAssetManager
    {
    public:
        TextureManager(IGPUResourceFactory& graphicsdevice);

        AssetHandle load(const std::string& filepath);
        ITexture* get(AssetHandle handle);
        void incrementRefCount(AssetHandle handle);
        void decrementRefCount(AssetHandle handle);
        void unloadUnused() override;
    private:
        std::unordered_map<uint64_t, std::shared_ptr<ITexture>> m_textures;
        std::unordered_map<uint64_t, AssetRecord> m_metadata;
        IGPUResourceFactory& m_graphicsdevice;
    };
}//namespace Lumos

