#pragma once
#include "assetbase.hpp"
#include <string_view>
#include "tiny_gltf_v3.h"

namespace Lumos
{
    class ModelImporter
    {
    public:
        ModelData import(const std::string& path);
    private:
        tg3_model parsemodel(const std::string& path, tg3_error_stack& errors);
        std::vector<TextureSource> loadTextures(const tg3_model& model);
        ModelData parseSubMeshes(const tg3_model& model);
    };
}//namespace Lumos
