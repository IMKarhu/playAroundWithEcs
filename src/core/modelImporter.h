#pragma once

#include "ecsImpl/components.h"
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
};
