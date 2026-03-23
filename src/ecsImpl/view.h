#pragma once
#include "componentpool.h"
#include <algorithm>

class Ecs;

template<typename... components>
class View
{
public:
    View(Ecs& ecs);

    struct Iterator {
        Ecs& ecs;
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
    Ecs& m_ecs;
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


#include "ecs.h"
template<typename... components>
View<components...>::View(Ecs& ecs) : m_ecs(ecs) {
    m_pools = { m_ecs.template getComponentPool<components>()... };

    if (!m_pools.empty()) {
        m_smallestpool = findSmallestPool();
    }
}

template<typename... components>
View<components...>::Iterator& View<components...>::Iterator::operator++() {
    index++;

    while(index < smallestpool->size() && !isValid()) {
        index++;
    }
    return *this;
}

template<typename... components>
bool View<components...>::Iterator::isValid() const {
    Entity ent = smallestpool->getEntity(index);

    return (ecs.template hasComponent<components>(ent) && ...);
}

template<typename... components>
Entity View<components...>::Iterator::operator*() const {
    return smallestpool->getEntity(index);
}

template<typename... components>
bool View<components...>::Iterator::operator !=(const Iterator& other) const {
    return index != other.index;
}

template<typename... components>
View<components...>::Iterator View<components...>::begin() {
    if (!m_smallestpool || m_smallestpool->size() == 0) {
        return end();
    }
   size_t first = 0;

    while(first < m_smallestpool->size() && !Iterator{m_ecs, m_smallestpool, first}.isValid()) {
        first++;
    }
    return Iterator{m_ecs, m_smallestpool, first};
}

template<typename... components>
View<components...>::Iterator View<components...>::end() {
    size_t endIndex = m_smallestpool ? m_smallestpool->size() : 0;
    return{m_ecs, m_smallestpool, endIndex};
}

