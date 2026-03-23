#pragma once
#include "componentpool.h"
#include "view.h"
#include <memory>
#include <typeindex>
#include <string>

// class View;
class Ecs
{
public:
    Ecs() = default;
    Ecs(const Ecs&) = delete;
    Ecs& operator=(const Ecs&) = delete;
    Ecs(Ecs&&) = default;
    Ecs& operator=(Ecs&&) = default;

    Entity createEntity(const std::string name = "Entity");
    size_t getNumOfAllEntities() const;

    template<typename T>
    bool registerComponentPool();

    template<typename T>
    ECS::ComponentPool<T>* getComponentPool();

    template<typename... components>
    auto view();

    template<typename T>
    void addComponent(Entity ent, const T& component);

    template<typename T>
    bool hasComponent(Entity ent);

    template<typename T>
    T& getComponent(Entity ent);

    template<typename T>
    void removeComponent(Entity ent);

private:
    std::unordered_map<std::type_index, std::unique_ptr<ECS::IComponentPool>> m_componentPoolMap;
    uint32_t m_nextEntity = 0;
    std::vector<Entity> m_entities;
};

template<typename T>
bool Ecs::registerComponentPool()
{
    std::type_index key = std::type_index(typeid(T));
    // std::println("key: {}", key.name());
    if (m_componentPoolMap.contains(key)) {
        // std::println("ComponentPool already exists!\n");
        return false;
    }
    m_componentPoolMap.insert({
            key,
            std::make_unique<ECS::ComponentPool<T>>()
            });
    return true;
}

template<typename T>
ECS::ComponentPool<T>* Ecs::getComponentPool() {
    auto it = m_componentPoolMap.find(std::type_index(typeid(T)));
    if(it == m_componentPoolMap.end()) {
        return nullptr;
    }
    return static_cast<ECS::ComponentPool<T>*>(it->second.get());
};

template<typename... components>
auto Ecs::view() {
    return View<components...>(*this);
}

template<typename T>
void Ecs::addComponent(Entity ent, const T& component) {
    registerComponentPool<T>();
    getComponentPool<T>()->add(ent, component);
}

template<typename T>
bool Ecs::hasComponent(Entity ent) {
    return getComponentPool<T>()->has(ent);
}

template<typename T>
T& Ecs::getComponent(Entity ent) {
    return getComponentPool<T>()->get(ent);
}

template<typename T>
void Ecs::removeComponent(Entity ent) {
    getComponentPool<T>()->remove(ent);
}

