#pragma once
#include "ecsImpl/ecs.h"
#include "ecsImpl/components.h"

enum class SceneState {
    Editor,
    Runtime
};

class Scene
{
public:
    Scene() = default;

    void destroyEntity(Entity entity) {} //empty implementation for now

    void setSceneState(SceneState state);
    SceneState sceneState() const;

    template<typename T>
    auto getAllEntitiesWithComponent()
    {
        return m_ecs.getComponentPool<T>()->entities();
    }

    Ecs& getEcs();
    Camera& getCamera();
    Transform& getCameraPosition();

private:
    SceneState m_state = SceneState::Editor;
    //temporary solution for having camera
    Camera m_editorCamera;
    Transform m_editorPosition;
    Ecs m_ecs;
};
