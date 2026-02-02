#include "window.h"
#include "ecs.h"
#include <cassert>
#include <cstdio>
#include <array>
#include <print>


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


int main()
{
    Window window;
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
    while(!window.shouldClose()) {
        window.pollEvents();
    }
    return 0;
}
