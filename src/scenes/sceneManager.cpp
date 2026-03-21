#include "sceneManager.h"


std::shared_ptr<Scene> SceneManager::newScene(std::string name)
{
    m_activeScene = std::make_shared<Scene>();
    return m_activeScene;
}

void SceneManager::play()
{
}

void SceneManager::stop()
{
}

std::shared_ptr<Scene> SceneManager::getActiveScene()
{
    return m_activeScene;
}
