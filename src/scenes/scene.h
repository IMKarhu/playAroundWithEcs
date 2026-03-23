#pragma once
#include "ecs.h"

enum class SceneState {
    Editor,
    Runtime
};

class Scene
{
public:
    Scene() = default;

    void destroyEntity(Entity entity) {} //empty implementation for now

    void updateEditor(float dt) {} //empty implementation for now
    void setSceneState(SceneState state);
    SceneState sceneState() const;

    template<typename T>
    auto getAllEntitiesWithComponent()
    {
        return m_ecs.getComponentPool<T>()->entities();
    }

    Ecs& getEcs();

private:
    SceneState m_state = SceneState::Editor;
    Ecs m_ecs;
};
