#include "scene.h"

void Scene::setSceneState(SceneState state)
{
    m_state = state;
}

SceneState Scene::sceneState() const
{
    return m_state;
}

Ecs& Scene::getEcs()
{
    return m_ecs;
}

Camera& Scene::getCamera()
{
    return m_editorCamera;
}

Transform& Scene::getCameraPosition()
{
    return m_editorPosition;
}

