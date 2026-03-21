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

