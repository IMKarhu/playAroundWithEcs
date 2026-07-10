#pragma once
#include "scene/scene.h"

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
    class CORE_API SceneSerializer
    {
    public:
        SceneSerializer(std::shared_ptr<Scene> scene);
        bool serialize(const std::string& filepath);
        bool deSerialize(const std::string& filepath, AssetManager& assetmanager);
    private:
        std::shared_ptr<Scene> m_scene;
    };
}// namespace Lumos
