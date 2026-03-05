#pragma once
#include "ecs.h"
#include <string>

enum class SceneState {
    Editor,
    Runtime
};

class Scene
{
public:
    Scene() = default;

    Entity createEntity(const std::string name = "Entity")
    {
        Entity ent = m_entities++;
        return ent;
    }

    void destroyEntity(Entity entity) {} //empty implementation for now

    void updateEditor(float dt) {} //empty implementation for now
    void setSceneState(SceneState state);
    SceneState sceneState() const;

    template<typename T>
    auto getAllEntitiesWithComponent()
    {
        return ECS::getComponentPool<T>()->entities();
    }
private:
    SceneState m_state = SceneState::Editor;
    uint32_t m_entities = 0;
};
