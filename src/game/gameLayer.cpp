#include "gameLayer.h"
#include "basePass.h"
#include "screenPass.h"
#include "ecsImpl/components.h"
#include "renderer.h"
#include "windowing/window.h"
#include "framebufferManager.h"
#include "framebufferBuilder.h"
#include <print>


GameLayer::GameLayer(Renderer& renderer, Window& window)
    : m_renderer(renderer), m_window(window), Layer("gamelayer")
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

    // auto [mesh, trans] = ModelImporter::importDataFromFIle("../src/game/assets/Sponza.gltf");
    // auto [mesh, trans] = ModelImporter::importDataFromFIle("../src/game/assets/FlightHelmet.gltf");
    // auto [mesh, trans] = ModelImporter::importDataFromFIle("../src/game/assets/Box.gltf");

    Mesh mesh;
    mesh.name = "Sponza.gltf";
    m_ecs.addComponent<Mesh>(cube, mesh);
    m_ecs.addComponent<Transform>(cube, {});

    auto screen = m_ecs.createEntity("screen");
    ScreenQuad screenquad;
    screenquad.name = "screen";
    m_ecs.addComponent<ScreenQuad>(screen, screenquad);

    auto camera = m_ecs.createEntity("camera");
    m_ecs.addComponent<Transform>(camera, {.position = glm::vec3(0.0, 0.0, 50.0)});
    m_ecs.addComponent<Camera>(camera, {.primary = true});

    m_framebuffermanager = std::make_unique<FrameBufferManager>();
    m_framebuffermanager->addFramebuffer("scene", FramebufferBuilder()
        .setSize(m_window.width(), m_window.height())
        .setSamples(1)
        .setColorAttachments({Format::RGBA8})
        .setDepthFormat(DepthFormat::Depth24Stencil8, DepthFormatType::Texture)
        .build());


    m_basepass = std::make_unique<BasePass>();
    m_screenpass = std::make_unique<ScreenPass>();

    m_assetmanager = std::make_unique<AssetManager>(m_renderer);
    m_assetmanager->loadAssetsFromDirectory("../src/game/assets/");
    std::println("size of entities: {}", m_ecs.getNumOfAllEntities());

}

void GameLayer::detach()
{
}

void GameLayer::update(float dt)
{
    m_renderer.beginFrame();
    RenderPassDesc desc;
    desc.framebuffer = "scene";
    //basepass
    m_renderer.beginPass(desc, *m_framebuffermanager.get());
    m_basepass->update(dt, m_ecs, m_renderer, *m_assetmanager.get());
    m_renderer.endPass(desc);
    m_renderer.flush(*m_assetmanager->getMeshManager());
    //screenpass
    desc.framebuffer = "";
    desc.depthtest = false;
    m_renderer.beginPass(desc, *m_framebuffermanager.get());
    m_screenpass->update(dt, m_ecs, m_renderer, *m_assetmanager.get(), *m_framebuffermanager.get());
    m_renderer.endPass(desc);
    m_renderer.flush(*m_assetmanager->getMeshManager());
    m_renderer.endFrame();
}

void GameLayer::event(const Event &event)
{
    if (event.type == EventType::KeyPress) {
        const auto &e = static_cast<const KeyEvent&>(event);
        for (auto ent : m_ecs.view<Camera, Transform>()) {
            auto& t = m_ecs.getComponent<Transform>(ent);
            if (e.key == 65 && (e.action == 1 ||  e.action == 2)) {
                t.position.x += 0.75f;
            }
            if (e.key == 68 && (e.action == 1 ||  e.action == 2)) {
                t.position.x -= 0.75f;
            }
            if (e.key == 87 && (e.action == 1 ||  e.action == 2)) {
                t.position.z -= 0.75f;
            }
            if (e.key == 83 && (e.action == 1 ||  e.action == 2)) {
                t.position.z += 0.75f;
            }
        }
    }

    if (event.type == EventType::WindowResize) {
        const auto &e = static_cast<const ResizeEvent&>(event);
        for (auto ent : m_ecs.view<Camera, Transform>()) {
            auto& camera = m_ecs.getComponent<Camera>(ent);
            if (camera.primary) {
                camera.aspectratio = (float)e.width/(float)e.height;
                m_framebuffermanager->resizeAll(e.width, e.height);
                break;
            }
        }
    }
}
