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
    class Texture : public IAsset
    {
    public:
        //this is the actual texture handle
        uint32_t m_renderid = 0;
        int m_width = 0;
        int m_height = 0;
        Texture(const std::string& path) {
            //TODO implement this
        }

        ~Texture() override {
            if (m_renderid != 0) {
                //TODO implemnt deletion
            }
        }
    };

    class CORE_API TextureManager : public ISubAssetManager
    {
    public:
        TextureManager(IGraphicsDevice& graphicsdevice);

        AssetHandle load(const std::string& filepath);
        ITexture* get(AssetHandle handle);
        void incrementRefCount(AssetHandle handle);
        void decrementRefCount(AssetHandle handle);
        void unloadUnused() override;
    private:
        std::unordered_map<uint64_t, std::shared_ptr<ITexture>> m_textures;
        std::unordered_map<uint64_t, AssetRecord> m_metadata;
        IGraphicsDevice& m_graphicsdevice;
    };
}//namespace Lumos

