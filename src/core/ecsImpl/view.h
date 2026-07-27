#pragma once
#include "componentpool.h"
#include <algorithm>
#include <print>

class Ecs;

template<typename EcsType = Ecs, typename... components>
class View
{
public:
    View(EcsType& ecs);

    struct Iterator {
        EcsType& ecs;
        ECS::IComponentPool* smallestpool;
        size_t index;

        Iterator& operator++();

        bool isValid() const;

        Entity operator*() const;

        bool operator !=(const Iterator& other) const;

    };

    Iterator begin();

    Iterator end();

private:
    EcsType& m_ecs;
    std::vector<ECS::IComponentPool*> m_pools;
    ECS::IComponentPool* m_smallestpool = nullptr;

    ECS::IComponentPool* findSmallestPool()
    {
        auto pool = *std::min_element(m_pools.begin(), m_pools.end(),
                [](ECS::IComponentPool* a, ECS::IComponentPool* b) {
                    return a->size() < b->size();
                });
        return pool;
    }
};


template<typename EcsType, typename... components>
View<EcsType, components...>::View(EcsType& ecs) : m_ecs(ecs) {
    m_pools = { m_ecs.template getComponentPool<components>()... };
    for (auto* pools : m_pools) {
        if (!pools) {
            m_smallestpool = nullptr;
            return;
        }
    }

    if (!m_pools.empty()) {
        m_smallestpool = findSmallestPool();
    }
}

template<typename EcsType, typename... components>
View<EcsType, components...>::Iterator& View<EcsType, components...>::Iterator::operator++() {
    index++;

    while(index < smallestpool->size() && !isValid()) {
        index++;
    }
    return *this;
}

template<typename EcsType, typename... components>
bool View<EcsType, components...>::Iterator::isValid() const {
    if (!smallestpool || index >= smallestpool->size()) {
        return false;
    }
    Entity ent = smallestpool->getEntity(index);

    return (ecs.template hasComponent<components>(ent) && ...);
}

template<typename EcsType, typename... components>
Entity View<EcsType, components...>::Iterator::operator*() const {
    return smallestpool->getEntity(index);
}

template<typename EcsType, typename... components>
bool View<EcsType, components...>::Iterator::operator !=(const Iterator& other) const {
    return index != other.index;
}

template<typename EcsType, typename... components>
View<EcsType, components...>::Iterator View<EcsType, components...>::begin() {
    if (!m_smallestpool || m_smallestpool->size() == 0) {
        return end();
    }
    uint32_t first = 0;

    while(first < m_smallestpool->size() && !Iterator{m_ecs, m_smallestpool, first}.isValid()) {
        first++;
    }
    return Iterator{m_ecs, m_smallestpool, first};
}

template<typename EcsType, typename... components>
View<EcsType, components...>::Iterator View<EcsType, components...>::end() {
    size_t endIndex = m_smallestpool ? m_smallestpool->size() : 0;
    return{m_ecs, m_smallestpool, endIndex};
}

