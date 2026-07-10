#pragma once
#include <string>

#include "ecsImpl/ecs.h"

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
    class AssetManager;
    class CORE_API Scene
    {
    public:
        Scene(AssetManager& assetmanager, const std::string& name = "scene");
        ~Scene() = default;

        Entity createEntity(std::string name);

        Ecs& getEcs();
    private:
        Ecs m_ecs;
        std::string m_name;
    };
}// namespace Lumos
