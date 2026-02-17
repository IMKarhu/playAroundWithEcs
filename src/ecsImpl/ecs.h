#ifndef ECS_H
#define ECS_H

#include <vector>
#include <cassert>
#include <cstdio>
#include <print>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <vector>



using Entity = uint32_t;
//maybe having this and the componentpool map as inline static is not the best option
//TODO: fix someday
inline std::vector<Entity> m_entities;

namespace ECS {

    class IComponentPool
    {
        public:
            virtual ~IComponentPool() = default;
        private:
    };

    template<typename T>
    class ComponentPool : public IComponentPool
    {
        public:
            ComponentPool<T>() {
            }

            inline void add(Entity ent, const T& component) {
                if(has(ent)) {
                    std::println("Component is already added");
                    return;
                }

                size_t size = m_components.size();
                m_entityToIndex[ent] = size;
                m_entities.push_back(ent);
                m_components.push_back(component);
            }

            inline void remove(Entity ent) {
                auto it = m_entityToIndex.find(ent);
                if(it == m_entityToIndex.end()) {
                    return;
                }
                size_t index = it->second;
                size_t last = m_components.size() - 1;
                Entity lastEnt = m_entities.at(last);

                m_components[index] = std::move(m_components.at(last));
                m_entities[index] = lastEnt;
                m_entityToIndex[lastEnt] = index;

                m_components.pop_back();
                m_entities.pop_back();
                m_entityToIndex.erase(ent);
            }

            inline bool has(Entity ent) const {
                return m_entityToIndex.contains(ent);
            }

            inline T& get(Entity ent) {
                return m_components[m_entityToIndex.at(ent)];
            }

            inline const std::vector<Entity>& entities() const {
                return m_entities;
            }

            inline void hello() {
                std::println("I am being called");
            }
        private:
            std::vector<T> m_components;
            std::vector<Entity> m_entities;
            std::unordered_map<Entity, size_t> m_entityToIndex;
    };

    //fix for now..
    inline std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_componentPoolMap;

    template<typename T>
    inline bool registerComponentPool() {
        //should be O(1), at the worst case O(n)
        std::type_index key = std::type_index(typeid(T));
        std::println("key: {}", key.name());
        if (m_componentPoolMap.contains(key)) {
            std::println("ComponentPool already exists!\n");
            return false;
        }
        m_componentPoolMap.insert({
                key,
                std::make_unique<ComponentPool<T>>()
                });
        return true;
    };

    template<typename T>
    inline ComponentPool<T>* getComponentPool() {
        auto it = m_componentPoolMap.find(std::type_index(typeid(T)));
        if(it == m_componentPoolMap.end()) {
            return nullptr;
        }
        return static_cast<ComponentPool<T>*>(it->second.get());
    };

    template<typename T>
    inline void addComponent(Entity ent, const T& component) {
        getComponentPool<T>()->add(ent, component);
    }

    template<typename T>
    inline bool hasComponent(Entity ent) {
        return getComponentPool<T>()->has(ent);
    }

    template<typename T>
    inline T& getComponent(Entity ent) {
        return getComponentPool<T>()->get(ent);
    }

    template<typename T>
    inline void removeComponent(Entity ent) {
        getComponentPool<T>()->remove(ent);
    }
}

#endif
