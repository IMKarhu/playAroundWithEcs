#pragma once
#include "textureManager.h"
#include "meshManager.h"
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
    class CORE_API AssetManager
    {
    public:
        AssetManager(IGraphicsDevice& graphicsdevice);

        void garbageCollect();
        TextureManager& getTextureManager();
        MeshManager& getMeshManager();
    private:
        std::unique_ptr<TextureManager> m_texturemanager;
        std::unique_ptr<MeshManager> m_meshmanager;
    };
}//namespace Lumos
