#pragma once

#include "ecsImpl/components.h"
#include "tiny_gltf_v3.h"
#include <string_view>
#include <string>

#ifdef _WIN32
    #ifdef CORE_EXPORT
    #define CORE_API __declspec(dllexport)
    #else
    #define CORE_API __declspec(dllimport)
    #endif
#else
    #define CORE_API
#endif


class CORE_API ModelImporter
{
public:
    struct Texture
    {
        std::string filename;
    };
    struct MaterialHandles
    {
        int32_t basecolorhandle = 0;
        int32_t normalhandle = 0;
        int32_t metallicroughnesshandle = 0;
        int32_t occlusionhandle = 0;
    };  
    struct ModelImportData
    {
        Mesh outMesh;
        Transform transform;
        std::vector<uint32_t> submeshmaterialindex;
        std::vector<MaterialHandles> materialhandles;
        std::vector<Texture> textures;
    };
    static ModelImportData importDataFromFile(std::string file);
    static ScreenQuad screenQuad();
private:
    static void parseModel(const tg3_model& model, ModelImportData& data);
    static void parseMeshe(const tg3_model& model, const tg3_mesh& mesh, ModelImportData& data);
    static void parseTextures(const tg3_image& image, ModelImportData& data);
    static void parseMaterials(const tg3_model& model, const tg3_material& material, MaterialHandles& materialhandle);
    static void parsePrimitive(const tg3_model& model, const tg3_primitive& primitive, ModelImportData& data, uint32_t submeshnum);
    static void readIndices(const tg3_model& model, const tg3_primitive& primitive, SubMesh& submesh);
    static void readAttributes(std::string_view view, const tg3_accessor& accessor, SubMesh& submesh);
};
