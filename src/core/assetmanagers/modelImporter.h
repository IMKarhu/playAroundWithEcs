#pragma once
#include "assetbase.hpp"
#include <vector>
#include "tiny_gltf_v3.h"

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
    class CORE_API ModelImporter
    {
    public:
        ModelImporter();
        ~ModelImporter();
        ModelData import(const std::string& path);
        std::vector<TextureSource> parseImages();
        void parseMaterials(ModelData& modeldata, std::vector<AssetHandle>& textures);
        void destroyctx();
    private:
        tg3_model m_model;
        tg3_error_stack m_errors;
        tg3_model parsemodel(const std::string& path);
        ModelData parseSubMeshes(const tg3_model& model);
    };
}//namespace Lumos
