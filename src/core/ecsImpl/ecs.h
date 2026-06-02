#pragma once
#include "componentpool.h"
#include "components.h"
#include <memory>
#include <typeindex>
#include <string>

class Ecs
{
public:
    Ecs() = default;
    Ecs(const Ecs&) = delete;
    Ecs& operator=(const Ecs&) = delete;
    Ecs(Ecs&&) = default;
    Ecs& operator=(Ecs&&) = default;

    Entity createEntity(const std::string name = "Entity")
    {
        Entity ent = m_nextEntity++;
        addComponent<Id>(ent, {name, ent});
        m_entities.emplace_back(ent);
        return ent;
    }
    size_t getNumOfAllEntities() const
    {
        return m_entities.size();
    }

    template<typename T>
    bool registerComponentPool();

    template<typename T>
    ECS::ComponentPool<T>* getComponentPool() const;

    template<typename... components>
    auto view() const;

    template<typename T>
    void addComponent(Entity ent, const T& component);

    template<typename T>
    bool hasComponent(Entity ent) const;

    template<typename T>
    T& getComponent(Entity ent) const;

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
        return false;
    }
    m_componentPoolMap.insert({
            key,
            std::make_unique<ECS::ComponentPool<T>>()
            });
    return true;
}

template<typename T>
ECS::ComponentPool<T>* Ecs::getComponentPool() const {
    auto it = m_componentPoolMap.find(std::type_index(typeid(T)));
    if(it == m_componentPoolMap.end()) {
        return nullptr;
    }
    return static_cast<ECS::ComponentPool<T>*>(it->second.get());
};

#include "view.h"
template<typename... components>
auto Ecs::view() const {
    return View<const Ecs, components...>(*this);
}

template<typename T>
void Ecs::addComponent(Entity ent, const T& component) {
    registerComponentPool<T>();
    getComponentPool<T>()->add(ent, component);
}

template<typename T>
bool Ecs::hasComponent(Entity ent) const {
    if (!getComponentPool<T>()) {
        return false;
    }
    return getComponentPool<T>()->has(ent);
}

template<typename T>
T& Ecs::getComponent(Entity ent) const {
    return getComponentPool<T>()->get(ent);
}

template<typename T>
void Ecs::removeComponent(Entity ent) {
    getComponentPool<T>()->remove(ent);
}

