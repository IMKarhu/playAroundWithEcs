#include "renderSystem.h"
#include "window.h"
#include "glrenderer.h"
#include "imguiLayer.h"
#include "components.h"
#include "framebuffer.h"
#include "framebufferBuilder.h"
#include "framebufferManager.h"

RenderSystem::RenderSystem(Window& window)
    : m_window(window)
{
    m_renderer = std::make_unique<GLRenderer>(window);
    m_uilayer = std::make_unique<ImguiLayer>(window);
    m_fb = FramebufferBuilder()
        .setSize(window.width(),window.height())
        .setSamples(1)
        .setColorAttachments({Format::RGBA8})
        .setDepthFormat(DepthFormat::Depth24Stencil8)
        .build();

    m_framebufferManager = std::make_shared<FrameBufferManager>();
}

RenderSystem::~RenderSystem() {}

void RenderSystem::initialize()
{
    std::println("initialize");
    auto pool = ECS::getComponentPool<Vertices>();
    auto ent = pool->entities();
    m_renderer->initialize(ent);
}

void RenderSystem::update(float dt)
{
    // m_renderer->beginFrame();
    m_framebufferManager->renderto(m_fb, [&](){
            m_renderer->renderScene(dt, m_fb->framebufferSpec().width, m_fb->framebufferSpec().height);
    });
    // m_renderer->renderScene(dt);
    // m_renderer->endFrame();
}

void RenderSystem::renderui()
{
    static bool open = true;
    m_uilayer->begin();
    m_uilayer->drawViewport(m_fb);
    ImguiLayer::beginWindow("settings", &open);
    ImguiLayer::endWindow();
    m_uilayer->end();


}

