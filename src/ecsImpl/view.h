#pragma once
#include "ecs.h"
#include <algorithm>

template<typename... components>
class View
{
public:
    View(Ecs& ecs) : m_ecs(ecs) {
        m_pools = { m_ecs.getComponentPool<components>()... };
        m_smallestpool = findSmallestPool();
    }

    struct Iterator {
        Ecs& ecs;
        ECS::IComponentPool* smallestpool;
        size_t index;

        Iterator& operator++() {
            index++;

            while(index < smallestpool->size() && !isValid()) {
                index++;
            }
            return *this;
        }

        bool isValid() const {
            Entity ent = smallestpool->getEntity(index);

            return (ecs.hasComponent<components>(ent) && ...);
        }

        Entity operator*() const {
            return smallestpool->getEntity(index);
        }

        bool operator !=(const Iterator& other) const {
            return index != other.index;
        }

    };

    Iterator begin() {
        size_t first = 0;

        while(first < m_smallestpool->size() && !Iterator{m_ecs, m_smallestpool, first}.isValid()) {
            first++;
        }
        return Iterator{m_ecs, m_smallestpool, first};
    }

    Iterator end() {
        return{m_ecs, m_smallestpool, m_smallestpool->size()};
    }

private:
    Ecs& m_ecs;
    std::vector<ECS::IComponentPool*> m_pools;
    ECS::IComponentPool* m_smallestpool;

    ECS::IComponentPool* findSmallestPool()
    {
        auto pool = *std::min_element(m_pools.begin(), m_pools.end(),
                [](ECS::IComponentPool* a, ECS::IComponentPool* b) {
                    return a->size() < b->size();
                });
        return pool;
    }
};
