#include "gameLayer.h"
#include "ecsImpl/components.h"
#include <print>


GameLayer::GameLayer(const Renderer &renderer)
    : m_renderer(renderer), Layer("gamelayer")
{
    EventDispatcher::subscribe(EventType::KeyPress, [this](const Event &e) {
            event(e);
    });
    EventDispatcher::subscribe(EventType::WindowResize, [this](const Event &e) {
            event(e);
    });
}

void GameLayer::attach()
{
    std::println("game layer attach");

    auto cube = m_ecs.createEntity("cube");
    m_ecs.addComponent<Transform>(cube, {});

    Mesh testcube;
    testcube.vertices = {
        // front
        {{-0.5f, -0.5f,  0.5f}, {1, 0, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 1, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}},
        {{-0.5f,  0.5f,  0.5f}, {1, 1, 1}},
        // back
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}},
        {{-0.5f, -0.5f, -0.5f}, {0, 1, 0}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {1, 1, 1}},
        // left
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
        {{-0.5f, -0.5f,  0.5f}, {0, 1, 0}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}},
        {{-0.5f,  0.5f, -0.5f}, {1, 1, 1}},
        // right
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {1, 1, 1}},
        // top
        {{-0.5f,  0.5f,  0.5f}, {1, 0, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}},
        {{-0.5f,  0.5f, -0.5f}, {1, 1, 1}},
        // bottom
        {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}},
        {{-0.5f, -0.5f,  0.5f}, {1, 1, 1}},
    };
    testcube.indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };
    testcube.name = "testcube";
    m_ecs.addComponent<Mesh>(cube, testcube);

    auto camera = m_ecs.createEntity("camera");
    m_ecs.addComponent<Transform>(camera, {.position = glm::vec3(0.0, 0.0, 10.0)});
    m_ecs.addComponent<Camera>(camera, {.primary = true});

    m_rendersystem = std::make_unique<RenderSystem>();
    m_rendersystem->initialize(m_ecs, m_renderer);
    std::println("size of entities: {}", m_ecs.getNumOfAllEntities());

}

void GameLayer::detach()
{
}

void GameLayer::update(float dt)
{
    m_rendersystem->update(dt, m_ecs, m_renderer);
}

void GameLayer::event(const Event &event)
{
    if (event.type == EventType::KeyPress) {
        const auto &e = static_cast<const KeyEvent&>(event);
        std::println("pressed a key: {} action: {} mods: {}",e.key,e.action, e.mods);
    }

    if (event.type == EventType::WindowResize) {
        const auto &e = static_cast<const ResizeEvent&>(event);
        for (auto ent : m_ecs.view<Camera, Transform>()) {
            auto& camera = m_ecs.getComponent<Camera>(ent);
            if (camera.primary) {
                camera.aspectratio = (float)e.width/(float)e.height;
                break;
            }
        }
    }
}
