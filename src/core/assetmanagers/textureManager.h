#pragma once
#include <unordered_map>
#include <string>
#include <memory>

#include "assetbase.hpp"

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
        TextureManager(IGPUResourceFactory& resourcefactory);

        AssetHandle load(const TextureSource& src);
        ITexture* get(AssetHandle handle);
        void unloadUnused() override;
    private:
        std::unordered_map<uint64_t, std::shared_ptr<ITexture>> m_textures;
        std::unordered_map<uint64_t, AssetRecord> m_metadata;
        IGPUResourceFactory& m_resourcefactory;
    };
}//namespace Lumos

