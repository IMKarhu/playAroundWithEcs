#include "renderSystem.h"
#include "window.h"
#include "glrenderer.h"
#include "imguiLayer.h"
#include "framebuffer.h"
#include "framebufferBuilder.h"
#include "framebufferManager.h"
#include "components.h"
#include "scene.h"

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

void RenderSystem::initialize(std::shared_ptr<Scene>& scene)
{
    std::println("initialize");
    //auto entities = scene->getAllEntitiesWithComponent<Vertices>();
    // auto& ecs = scene->getEcs();
    // Entity ent = ecs.createEntity("entity1");
    // Vertices verts;
    // verts.m_vertices = {
    //     // front
    //     {{-0.5f, -0.5f,  0.5f}, {1, 0, 0}},
    //     {{ 0.5f, -0.5f,  0.5f}, {0, 1, 0}},
    //     {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}},
    //     {{-0.5f,  0.5f,  0.5f}, {1, 1, 1}},
    //     // back
    //     {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}},
    //     {{-0.5f, -0.5f, -0.5f}, {0, 1, 0}},
    //     {{-0.5f,  0.5f, -0.5f}, {0, 0, 1}},
    //     {{ 0.5f,  0.5f, -0.5f}, {1, 1, 1}},
    //     // left
    //     {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
    //     {{-0.5f, -0.5f,  0.5f}, {0, 1, 0}},
    //     {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}},
    //     {{-0.5f,  0.5f, -0.5f}, {1, 1, 1}},
    //     // right
    //     {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}},
    //     {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}},
    //     {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}},
    //     {{ 0.5f,  0.5f,  0.5f}, {1, 1, 1}},
    //     // top
    //     {{-0.5f,  0.5f,  0.5f}, {1, 0, 0}},
    //     {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}},
    //     {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}},
    //     {{-0.5f,  0.5f, -0.5f}, {1, 1, 1}},
    //     // bottom
    //     {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
    //     {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}},
    //     {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}},
    //     {{-0.5f, -0.5f,  0.5f}, {1, 1, 1}},
    // };
    // verts.m_indices = {
    //     0, 1, 2, 2, 3, 0,
    //     4, 5, 6, 6, 7, 4,
    //     8, 9, 10, 10, 11, 8,
    //     12, 13, 14, 14, 15, 12,
    //     16, 17, 18, 18, 19, 16,
    //     20, 21, 22, 22, 23, 20
    // };
    //
    // ecs.addComponent<Vertices>(ent, verts);
    // for (auto entity : ecs.view<Vertices>()) {
    //     auto& vert = ecs.getComponent<Vertices>(entity);
    //     glCreateVertexArrays(1, &vert.m_vao);
    //     glCreateBuffers(1, &vert.m_vbo);
    //     glCreateBuffers(1, &vert.m_ebo);
    //
    //     glNamedBufferData(vert.m_vbo,
    //             vert.m_vertices.size() * sizeof(Vertex),
    //             vert.m_vertices.data(),
    //             GL_STATIC_DRAW
    //     );
    //
    //     glNamedBufferData(vert.m_ebo,
    //             vert.m_indices.size() * sizeof(uint32_t),
    //             vert.m_indices.data(),
    //             GL_STATIC_DRAW
    //     );
    //
    //     glVertexArrayVertexBuffer(vert.m_vao, 0, vert.m_vbo, 0, sizeof(Vertex));
    //     glVertexArrayElementBuffer(vert.m_vao, vert.m_ebo);
    //
    //     glEnableVertexArrayAttrib(vert.m_vao, 0);
    //     glVertexArrayAttribFormat(vert.m_vao,
    //             0,
    //             3,
    //             GL_FLOAT,
    //             GL_FALSE,
    //             offsetof(Vertex, position)
    //     );
    //     glVertexArrayAttribBinding(vert.m_vao, 0, 0);
    //
    //     glEnableVertexArrayAttrib(vert.m_vao, 1);
    //     glVertexArrayAttribFormat(vert.m_vao,
    //             1,
    //             3,
    //             GL_FLOAT,
    //             GL_FALSE,
    //             offsetof(Vertex, color)
    //     );
    //     glVertexArrayAttribBinding(vert.m_vao, 1, 0);
    // }

    // auto pool = ECS::getComponentPool<Vertices>();
    // auto ent = pool->entities();
}

void RenderSystem::update(float dt, std::shared_ptr<Scene>& scene)
{
    auto& ecs = scene->getEcs();
    std::println("number of all entities: {}", ecs.getNumOfAllEntities());
    m_framebufferManager->renderto(m_fb, [&](){
            m_renderer->renderScene(dt, m_fb->framebufferSpec().width, m_fb->framebufferSpec().height);
    });
}

void RenderSystem::renderui(std::shared_ptr<Scene>& scene)
{
    static bool open = true;
    m_uilayer->begin();
    m_uilayer->drawViewport(m_fb);
    m_uilayer->drawSceneGraph(scene);
    ImguiLayer::beginWindow("settings", &open);
    ImguiLayer::endWindow();
    m_uilayer->end();
}

