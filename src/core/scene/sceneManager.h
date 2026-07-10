#pragma once
#include <memory>
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
    class CORE_API SceneManager
    {
    public:
        void setActiveScene(std::shared_ptr<Scene> scene);
        std::shared_ptr<Scene> getActiveScene();
    private:
        std::shared_ptr<Scene> m_activescene = nullptr;
    };
}// namespace Lumos
