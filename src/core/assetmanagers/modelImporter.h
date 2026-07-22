#pragma once
#include "assetbase.hpp"
#include <vector>
#include <string_view>
#include "tiny_gltf_v3.h"
#include <glm/glm.hpp>

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
    struct ModelImportMaterialIndexes
    {
        int32_t basecolor = -1;
        int32_t normal = -1;
        int32_t metrough = -1;
    };

    struct ModelImportSubMeshData
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        ModelImportMaterialIndexes indexes;
    };

    ///struct containing all data from gltf/glb file
    struct ModelImportData
    {
        std::vector<ModelImportSubMeshData> submeshes;
        std::vector<TextureSource> texsources;
    };
    class CORE_API ModelImporter
    {
    public:
        ModelImporter();
        ~ModelImporter();
        ModelImportData import(const std::string& path);
        std::vector<TextureSource> parseImages();
        void parseMaterials(ModelImportData& modeldata);
        void destroyctx();
    private:
        tg3_model m_model;
        tg3_error_stack m_errors;
        enum class VertexAttributeType
        {
            Position,
            Texcoord,
            Normal,
            Tangent,
            Unknown
        };
        tg3_model parsemodel(const std::string& path);
        ModelImportData parseSubMeshes(const tg3_model& model);
        void parseIndices(ModelImportData& data, const tg3_model& model);
        VertexAttributeType attributeType(std::string_view name);
        glm::vec3 readPositions(const float* offset);
        glm::vec2 readTexcoords(const float* offset);
        glm::vec3 readNormals(const float* offset);
        glm::vec4 readTangents(const float* offset);
    };
}//namespace Lumos
