#include "gameLayer.h"
#include "windowing/eventDispatcher.h"
#include "windowing/window.h"
#include "renderer.h"
#include "ecsImpl/ecs.h"
#include "ecsImpl/components.h"
#include "framebufferBuilder.h"
#include "assetmanagers/assetManager.h"
#include "scene/scene.h"
#include "lookupTables.h"

#include <print>
#include <array>
#include <glm/gtc/matrix_transform.hpp>

GameLayer::GameLayer(Lumos::Renderer& renderer, Lumos::AssetManager& assetmanager, const Lumos::Window& window)
    : m_renderer(renderer)
    , m_assetmanager(assetmanager)
    , m_window(window)
    , Layer("gamelayer")
{
}

void GameLayer::attach()
{
    std::println("game layer attach");

    m_framebuffermanager = std::make_unique<Lumos::FrameBufferManager>();
    m_framebuffermanager->addFramebuffer("gbuffer", Lumos::FramebufferBuilder()
        .setSize(m_window.width(), m_window.height())
        .setSamples(1)
        .setColorAttachments({Lumos::Format::RGBA8,
                              Lumos::Format::RGBA16F,
                              Lumos::Format::RGBA16F})
        .setDepthFormat(Lumos::DepthFormat::Depth24Stencil8, Lumos::DepthFormatType::Texture)
        .build());

    m_framebuffermanager->addFramebuffer("lighting", Lumos::FramebufferBuilder()
            .setSize(m_window.width(), m_window.height())
            .setSamples(1)
            .setColorAttachments({Lumos::Format::RGBA16F})
            .setDepthFormat(Lumos::DepthFormat::Depth24Stencil8, Lumos::DepthFormatType::Texture)
            .build());

    m_basepass = std::make_unique<BasePass>();
    m_screenpass = std::make_unique<ScreenPass>();
    m_lightingpass = std::make_unique<LightingPass>();
}

void GameLayer::detach()
{
}

void GameLayer::update(float dt, const std::shared_ptr<Lumos::Scene>& scene)
{
    if(!scene) {
        std::println("scene does not exist. return from gameLayer update");
        return;
    }
    auto& ecs = scene->getEcs();
    auto renderpass = [&]() {
        // std::println("in renderpass");
    };
    updateTransform(dt, ecs);

    m_renderer.beginFrame();
    Lumos::RenderPassDesc desc;
    desc.framebuffer = "gbuffer";
    //basepass
    m_renderer.beginPass(desc, *m_framebuffermanager.get());
    m_basepass->update(ecs, m_renderer, m_assetmanager);
    m_renderer.flush(renderpass, m_assetmanager);
    m_renderer.endPass();
    //lightingpass
    desc.name = "lighting";
    desc.framebuffer = "lighting";
    desc.depthtest = false;
    m_renderer.beginPass(desc, *m_framebuffermanager.get());
    m_lightingpass->update(ecs, m_renderer, *m_framebuffermanager.get());
    m_renderer.flush(renderpass, m_assetmanager);
    m_renderer.endPass();
    //screenpass
    desc.framebuffer = "";
    desc.depthtest = false;
    m_renderer.beginPass(desc, *m_framebuffermanager.get());
    // not really sure if this will stay as passing the colorAttachment id to screen but we need to somehow
    // get it to renderer
    m_screenpass->update(ecs, m_renderer, m_framebuffermanager->getFramebuffer("lighting")->colorAttachment(0).id);
    m_renderer.flush(renderpass, m_assetmanager);
    m_renderer.endPass();
    m_renderer.endFrame();
}

void GameLayer::event(Lumos::Event& event, const std::shared_ptr<Lumos::Scene>& scene)
{
    auto& ecs = scene->getEcs();
    Lumos::EventDispatcher dispatcher(event);
    dispatcher.dispatch<Lumos::WindowResizeEvent>([this, &ecs](Lumos::WindowResizeEvent& e) {
            m_renderer.setDefaultFramebufferDimensios(e.width(), e.height());
            m_framebuffermanager->resizeAll(e.width(), e.height());
            for (auto ent : ecs.view<Lumos::Camera, Lumos::Transform>()) {
                auto& camera = ecs.getComponent<Lumos::Camera>(ent);
                if (camera.primary) {
                    camera.aspectratio = (float)e.width()/(float)e.height();
                    break;
                }
            }
            return false;
    });
    
    dispatcher.dispatch<Lumos::KeyPressedEvent>([this, &ecs](Lumos::KeyPressedEvent& e) {
            //this has some jank in it, ie if you press two keys at the same time only one gets processed
            //and if you hold one key and press another it fires the other one and treats the other one being "released"
            //probably has more to do how we handle state in window keycallback than what we do here
            for (auto ent : ecs.view<Lumos::Camera, Lumos::Movement>()) {
                auto& m = ecs.getComponent<Lumos::Movement>(ent);
                if (e.button() == 65) {
                    m.moved = true;
                    m.button = 65;
                }
                if (e.button() == 68) {
                    m.moved = true;
                    m.button = 68;
                }
                if (e.button() == 87) {
                    m.moved = true;
                    m.button = 87;
                }
                if (e.button() == 83) {
                    m.moved = true;
                    m.button = 83;
                }
                if (e.button() == 69) {//e
                    m.moved = true;
                    m.button = 69;
                }
                if (e.button() == 81) {//q
                    m.moved = true;
                    m.button = 81;
                }
            }
            return false;
    });
}

void GameLayer::updateTransform(float dt, Ecs& ecs)
{
     for (auto ent : ecs.view<Lumos::Camera, Lumos::Movement, Lumos::Transform>()) {
         auto& m = ecs.getComponent<Lumos::Movement>(ent);
         auto& t = ecs.getComponent<Lumos::Transform>(ent);
         if (m.button == 65) {
             t.position.x += m.speed* dt;
             m.moved = false;
             m.button = -1;
         }
         if (m.button == 68) {
             t.position.x -= m.speed * dt;
             m.moved = false;
             m.button = -1;
         }
         if (m.button == 87) {
             t.position.z -= m.speed * dt;
             m.moved = false;
             m.button = -1;
         }
         if (m.button == 83) {
             t.position.z += m.speed * dt;
             m.moved = false;
             m.button = -1;
         }
         if (m.button == 69) {//e
             t.position.y += m.speed * dt;
             m.moved = false;
             m.button = -1;
         }
         if (m.button == 81) {//q
             t.position.y -= m.speed * dt;
             m.moved = false;
             m.button = -1;
         }
     }

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);

    for (auto ent : ecs.view<Lumos::Transform, Lumos::Camera>()) {
        auto& camtransform = ecs.getComponent<Lumos::Transform>(ent);
        auto& camera = ecs.getComponent<Lumos::Camera>(ent);
        if (camera.primary) {
            view = glm::lookAt(camtransform.position, camtransform.position + glm::vec3(0.0f,0.0f, -1.0f), camera.upvector);
            proj = glm::perspective(glm::radians(camera.fov), camera.aspectratio, camera.nearclip, camera.farclip);
            break;
        }
    }

    for (auto ent : ecs.view<Lumos::Transform, Lumos::MeshComponent>()) {
        auto& transform = ecs.getComponent<Lumos::Transform>(ent);
        auto& mesh = ecs.getComponent<Lumos::MeshComponent>(ent);
        // transform.scale = glm::vec3(0.00800000037997961,
        //                             0.00800000037997961,
        //                             0.00800000037997961);
        transform.model = glm::translate(glm::mat4(1.0f), transform.position);
        transform.model = glm::rotate(transform.model, glm::radians(20.0f * dt), glm::vec3(0.0, 1.0, 0.0));
        transform.model = glm::scale(transform.model, glm::vec3(transform.scale));
        transform.mvp  = proj * view;
    }
}

