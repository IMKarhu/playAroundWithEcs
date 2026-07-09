#include "gameLayer.h"
#include "basePass.h"
#include "screenPass.h"
#include "lightingPass.h"
#include "ecsImpl/components.h"
#include "renderer.h"
#include "windowing/window.h"
#include "framebufferManager.h"
#include "framebufferBuilder.h"
#include "assetManager.h"
#include <print>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer(Renderer& renderer, Lumos::AssetManager& assetmanager, Window& window)
    : m_renderer(renderer)
    , m_assetmanager(assetmanager)
    , m_window(window)
    , Layer("gamelayer")
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

    Lumos::AssetHandle sponza = m_assetmanager.getMeshManager().load("../src/game/assets/FlightHelmet.gltf");
    Mesh mesh;
    mesh.name = "world";
    mesh.assethandle = sponza;
    m_ecs.addComponent<Mesh>(cube, mesh);
    m_ecs.addComponent<Transform>(cube, {});

    auto screen = m_ecs.createEntity("screen");
    ScreenQuad screenquad;
    screenquad.name = "screen";
    m_ecs.addComponent<ScreenQuad>(screen, screenquad);

    auto camera = m_ecs.createEntity("camera");
    m_ecs.addComponent<Transform>(camera, {.position = glm::vec3(0.0, 0.0, 10.0)});
    m_ecs.addComponent<Camera>(camera, {.primary = true});

    auto light = m_ecs.createEntity("directional light");
    m_ecs.addComponent<Transform>(light, {.position = glm::vec3(0.0, 10.0, 0.0)});
    m_ecs.addComponent<Light>(light, {});

    m_framebuffermanager = std::make_unique<FrameBufferManager>();
    m_framebuffermanager->addFramebuffer("gbuffer", FramebufferBuilder()
        .setSize(m_window.width(), m_window.height())
        .setSamples(1)
        .setColorAttachments({Format::RGBA8,
                              Format::RGBA16F,
                              Format::RGBA16F})
        .setDepthFormat(DepthFormat::Depth24Stencil8, DepthFormatType::Texture)
        .build());

    m_framebuffermanager->addFramebuffer("lighting", FramebufferBuilder()
            .setSize(m_window.width(), m_window.height())
            .setSamples(1)
            .setColorAttachments({Format::RGBA16F})
            .setDepthFormat(DepthFormat::Depth24Stencil8, DepthFormatType::Texture)
            .build());

    m_basepass = std::make_unique<BasePass>();
    m_screenpass = std::make_unique<ScreenPass>();
    m_lightingpass = std::make_unique<LightingPass>();

    std::println("size of entities: {}", m_ecs.getNumOfAllEntities());
}

void GameLayer::detach()
{
}

void GameLayer::update(float dt)
{
    auto renderpass = [&]() {
        // std::println("in renderpass");
    };
    updateTransform(dt);

    m_renderer.beginFrame();
    RenderPassDesc desc;
    desc.framebuffer = "gbuffer";
    //basepass
    m_renderer.beginPass(desc, *m_framebuffermanager.get());
    m_basepass->update(m_ecs, m_renderer, m_assetmanager);
    m_renderer.flush(renderpass, m_assetmanager);
    m_renderer.endPass();
    //lightingpass
    desc.name = "lighting";
    desc.framebuffer = "lighting";
    desc.depthtest = false;
    m_renderer.beginPass(desc, *m_framebuffermanager.get());
    m_lightingpass->update(m_ecs, m_renderer, *m_framebuffermanager.get());
    m_renderer.flush(renderpass, m_assetmanager);
    m_renderer.endPass();
    //screenpass
    desc.framebuffer = "";
    desc.depthtest = false;
    m_renderer.beginPass(desc, *m_framebuffermanager.get());
    // not really sure if this will stay as passing the colorAttachment id to screen but we need to somehow
    // get it to renderer
    m_screenpass->update(m_ecs, m_renderer, m_framebuffermanager->getFramebuffer("lighting")->colorAttachment(0).id);
    m_renderer.flush(renderpass, m_assetmanager);
    m_renderer.endPass();
    m_renderer.endFrame();
}

void GameLayer::event(const Event &event)
{
    if (event.type == EventType::KeyPress) {
        const auto &e = static_cast<const KeyEvent&>(event);
        // std::println("key: {} action: {}", e.key, e.action);
        for (auto ent : m_ecs.view<Camera, Transform>()) {
            auto& t = m_ecs.getComponent<Transform>(ent);
            if (e.key == 65 && (e.action == 1 ||  e.action == 2)) {
                t.position.x += 0.75f * 0.1;
            }
            if (e.key == 68 && (e.action == 1 ||  e.action == 2)) {
                t.position.x -= 0.75f * 0.1;
            }
            if (e.key == 87 && (e.action == 1 ||  e.action == 2)) {
                t.position.z -= 0.75f * 0.1;
            }
            if (e.key == 83 && (e.action == 1 ||  e.action == 2)) {
                t.position.z += 0.75f * 0.1;
            }
            if (e.key == 69 && (e.action == 1 || e.action == 2)) {//e
                t.position.y += 0.75f * 0.1;
            }
            if (e.key == 81 && (e.action == 1 || e.action == 2)) {//q
                t.position.y -= 0.75f * 0.1;
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

void GameLayer::updateTransform(float dt)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    for (auto ent : m_ecs.view<Transform, Camera>()) {
        auto& camtransform = m_ecs.getComponent<Transform>(ent);
        auto& camera = m_ecs.getComponent<Camera>(ent);
        if (camera.primary) {

            view = glm::lookAt(camtransform.position, camtransform.position + glm::vec3(0.0f,0.0f, -1.0f), camera.upvector);
            proj = glm::perspective(glm::radians(camera.fov), camera.aspectratio, camera.nearclip, camera.farclip);
            break;
        }
    }

    for (auto ent : m_ecs.view<Transform, Mesh>()) {
        auto& transform = m_ecs.getComponent<Transform>(ent);
        auto& mesh = m_ecs.getComponent<Mesh>(ent);
        // transform.scale = glm::vec3(0.00800000037997961,
        //                             0.00800000037997961,
        //                             0.00800000037997961);
        transform.model = glm::translate(glm::mat4(1.0f), transform.position);
        transform.model = glm::rotate(transform.model, glm::radians(20.0f * dt), glm::vec3(0.0, 1.0, 0.0));
        transform.model = glm::scale(transform.model, glm::vec3(transform.scale));
        transform.mvp  = proj * view;
    }
}
