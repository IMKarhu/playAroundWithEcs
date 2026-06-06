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
    static Mesh importDataFromFIle(std::string file);
private:
};
