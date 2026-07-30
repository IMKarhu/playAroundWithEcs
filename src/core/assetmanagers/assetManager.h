#pragma once
#include "textureManager.h"
#include "meshManager.h"
#include "materialManager.h"
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
    class CORE_API AssetManager
    {
    public:
        AssetManager(IGPUResourceFactory& graphicsfactory);

        void garbageCollect();
        TextureManager& getTextureManager();
        MeshManager& getMeshManager();
        MaterialManager& getMaterialManager();
    private:
        std::unique_ptr<TextureManager> m_texturemanager;
        std::unique_ptr<MeshManager> m_meshmanager;
        std::unique_ptr<MaterialManager> m_materialmanager;
    };
}//namespace Lumos
