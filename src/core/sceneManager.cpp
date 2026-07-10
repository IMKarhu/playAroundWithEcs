#include "sceneManager.h"

namespace Lumos
{
    void SceneManager::setActiveScene(std::shared_ptr<Scene> scene)
    {
        m_activescene = scene;
    }

    std::shared_ptr<Scene> SceneManager::getActiveScene()
    {
        return m_activescene;
    }

}// namespace Lumos
