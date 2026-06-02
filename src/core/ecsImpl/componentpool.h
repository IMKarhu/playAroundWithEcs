#pragma once
#include <vector>
#include <cstdio>
#include <print>
#include <unordered_map>
#include <vector>

using Entity = uint32_t;

namespace ECS {

    class IComponentPool
    {
        public:
            virtual ~IComponentPool() = default;
            virtual size_t size() const = 0;
            virtual Entity getEntity(size_t index) = 0;
        private:
    };

    template<typename T>
    class ComponentPool : public IComponentPool
    {
        public:
            ComponentPool<T>() {
            }

            size_t size() const override { return m_components.size(); }

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

            inline Entity getEntity(size_t index) override {
                return m_entities.at(index);
            }

            inline const std::vector<Entity>& entities() const {
                return m_entities;
            }

        private:
            std::vector<T> m_components;
            std::vector<Entity> m_entities;
            std::unordered_map<Entity, size_t> m_entityToIndex;
    };
}
