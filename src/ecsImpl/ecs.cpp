#include "ecs.h"
#include "components.h"

Entity Ecs::createEntity(const std::string name)
{
    Entity ent = m_nextEntity++;
    addComponent<Id>(ent, {name, ent});
    m_entities.emplace_back(ent);
    return ent;
}

size_t Ecs::getNumOfAllEntities() const
{
    return m_entities.size();
}
