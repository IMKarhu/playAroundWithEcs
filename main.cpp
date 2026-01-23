#include <cassert>
#include <cstdio>
#include <array>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <print>
#include <utility>
#include <vector>


using Entity = uint32_t;
std::vector<Entity> m_entities;

//Fine for now to limit max amount.
//In reality there should not be any kind of entity limitations
const uint32_t MAXENTITIES = 5000;

struct Transform
{
    std::array<float, 3> m_position;
    std::array<float, 3> m_rotation;
    std::array<float, 3> m_scale;
};

struct Sprite
{
    float a;
};

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
                std::println("ctor called");
            }

            void add(Entity ent, const T& component) {
                if(has(ent)) {
                    std::println("Component is already added");
                    return;
                }

                size_t size = m_components.size();
                m_entityToIndex[ent] = size;
                m_entities.push_back(ent);
                m_components.push_back(component);
            }

            void remove(Entity ent) {
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

            bool has(Entity ent) const {
                return m_entityToIndex.contains(ent);
            }

            T& get(Entity ent) {
                return m_components[m_entityToIndex.at(ent)];
            }

            const std::vector<Entity>& entities() const {
                return m_entities;
            }

            void hello() {
                std::println("I am being called");
            }
        private:
            std::vector<T> m_components;
            std::vector<Entity> m_entities;
            std::unordered_map<Entity, size_t> m_entityToIndex;
    };

    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_componentPoolMap;

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
                std::make_unique<ComponentPool<T>>()
                });
        return true;
    };

    template<typename T>
    ComponentPool<T>* getComponentPool() {
        auto it = m_componentPoolMap.find(std::type_index(typeid(T)));
        if(it == m_componentPoolMap.end()) {
            return nullptr;
        }
        return static_cast<ComponentPool<T>*>(it->second.get());
    };

    template<typename T>
    void addComponent(Entity ent, const T& component) {
        getComponentPool<T>()->add(ent, component);
    }

    template<typename T>
    bool hasComponent(Entity ent, const T& component) {
        return getComponentPool<T>()->has(ent);
    }

    template<typename T>
    T& getComponent(Entity ent, const T& component) {
        return getComponentPool<T>()->get(ent);
    }

    template<typename T>
    void removeComponent(Entity ent, const T& component) {
        getComponentPool<T>()->remove(ent);
    }
}



int main()
{
    bool ok = ECS::registerComponentPool<Transform>();
    assert(ok);
    ok = ECS::registerComponentPool<Sprite>();
    assert(ok);

    for(size_t i = 0; i < 4000; ++i) {
        Entity ent = 1;
        m_entities.push_back(ent);
        ent++;
    }

    for(size_t i = 0; i < m_entities.size(); ++i) {
        ECS::addComponent<Transform>(i, {{0,0,0},{0,0,0},{1,1,1}});
    }
    auto transformPool = ECS::getComponentPool<Transform>();
    auto entt = transformPool->entities();
    std::println("size of entities {}", entt.size());
    return 0;
}
