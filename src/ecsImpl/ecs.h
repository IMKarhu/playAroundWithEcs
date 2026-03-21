#pragma once
#include "componentpool.h"
#include <memory>
#include <typeindex>
#include <string>

class Ecs
{
public:
    template<typename T>
    bool registerComponentPool() {
        //should be O(1), at the worst case O(n)
        std::type_index key = std::type_index(typeid(T));
        std::println("key: {}", key.name());
        if (m_componentPoolMap.contains(key)) {
            std::println("ComponentPool already exists!\n");
            return false;
        }
        m_componentPoolMap.insert({
                key,
                std::make_unique<ECS::ComponentPool<T>>()
                });
        return true;
    };

    template<typename T>
    ECS::ComponentPool<T>* getComponentPool() {
        auto it = m_componentPoolMap.find(std::type_index(typeid(T)));
        if(it == m_componentPoolMap.end()) {
            return nullptr;
        }
        return static_cast<ECS::ComponentPool<T>*>(it->second.get());
    };

    template<typename... components>
    auto view() {
        return View<components...>(*this);
    }

    template<typename T>
    void addComponent(Entity ent, const T& component) {
        registerComponentPool<T>();
        getComponentPool<T>()->add(ent, component);
    }

    template<typename T>
    bool hasComponent(Entity ent) {
        return getComponentPool<T>()->has(ent);
    }

    template<typename T>
    T& getComponent(Entity ent) {
        return getComponentPool<T>()->get(ent);
    }

    template<typename T>
    void removeComponent(Entity ent) {
        getComponentPool<T>()->remove(ent);
    }

    Entity createEntity(const std::string name = "Entity");

private:
    std::unordered_map<std::type_index, std::unique_ptr<ECS::IComponentPool>> m_componentPoolMap;
    uint32_t m_nextEntity = 0;
    std::vector<Entity> m_entities;
};
