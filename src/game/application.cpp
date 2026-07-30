#include "application.h"
#include "opengl/glrenderer.h"
#include "windowing/eventDispatcher.h"
#include "gameLayer.h"
#include "scene/scene.h"
#include "scene/sceneSerializer.h"
#include "lookupTables.h"
#include <chrono>
#include <format>

Application::Application(Lumos::Platform platform)
{
    m_window = std::make_unique<Lumos::Window>();
    m_window->setEventCallback(std::bind(&Application::event, this, std::placeholders::_1));
    //this can be glrenderer for now since thats all we have
    m_renderer = std::make_shared<Lumos::GLRenderer>();
    //quick fix for setting framebuffer width and height so I dont need to pass window to renderer
    m_renderer->setDefaultFramebufferDimensios(m_window->width(), m_window->height());
    m_assetmanager = std::make_unique<Lumos::AssetManager>(m_renderer->getGPUResourceFactory());

    pushLayer(std::make_unique<GameLayer>(*m_renderer, *m_assetmanager, *m_window));
    m_renderer->createAndAddToShaderCache("screen","shaders/screen.vert", "shaders/screen.frag");
    m_renderer->createAndAddToShaderCache("basePass", "shaders/shader.vert", "shaders/shader.frag");
    m_renderer->createAndAddToShaderCache("lighting", "shaders/lighting.vert", "shaders/lighting.frag");

    auto Scene = std::make_shared<Lumos::Scene>(*m_assetmanager, "test scene");
    Lumos::SceneSerializer serializer(Scene);
    bool success = serializer.deSerialize("../src/game/scenes/testscene.json", *m_assetmanager);
    if(!success) {
        std::println("failed to load scene");
    }
    m_scenemanager.setActiveScene(Scene);
    // marchingCubes();
}

void Application::run()
{
    auto start = std::chrono::high_resolution_clock::now();

    float framecount = 0.0f;
    float timeaccumulator = 0.0f;
    float curfps = 0.0f;
    while(!m_window->shouldClose()) {
        auto current = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(current - start).count();
        start = current;

        timeaccumulator += dt;
        framecount += 1.0f;

        if (timeaccumulator >= 1.0f) {
            curfps = framecount / timeaccumulator;
            m_titleAndfps = std::format("Lumos engine test. Fps: {:.1f}", curfps);
            m_window->setWindowTitle(m_titleAndfps);
            framecount = 0.0f;
            timeaccumulator = 0.0f;
        }

        for (auto& layer : m_layers) {
            layer->update(dt, m_scenemanager.getActiveScene());
        }

        m_window->pollEvents();
        m_window->swapBuffers();
    }
}

void Application::pushLayer(std::unique_ptr<Lumos::Layer> layer)
{
    layer->attach();
    m_layers.push_back(std::move(layer));
}

void Application::event(Lumos::Event& event)
{
    Lumos::EventDispatcher dispatcher(event);

    dispatcher.dispatch<Lumos::WindowCloseEvent>([this](Lumos::WindowCloseEvent& e) {
            m_window->setWindowShouldClose(true);
            return true;
    });

    for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it) {
        if (event.handled) {
            break;
        }
        (*it)->event(event, m_scenemanager.getActiveScene());
    }
}

size_t Application::index(uint32_t x, uint32_t y, uint32_t z)
{
    return x + y * samples + z * (samples * samples);
}

glm::vec3 Application::linearInterpolation(const glm::vec3& corner0, const glm::vec3& corner1, float density0, float density1)
{
    glm::vec3 p0 = glm::vec3(corner0);
    glm::vec3 p1 = glm::vec3(corner1);

    float t = -density0 / (density1 - density0);
    return glm::mix(p0, p1, t);
}

void Application::printDensity()
{
    for (size_t y = 0; y < samples; ++y) {
        std::println("==== y = {} ====", y);
        for (size_t z = 0; z < samples; ++z) {
            for (size_t x = 0; x < samples; ++x) {
                std::print("{:6.1f}", density[index(x, y, z)]);
            }
            std::println();
        }
        std::println();
    }
}

void Application::marchingCubes()
{
    float radius = SIZE * 0.4f;
    marchingcubesdata.submeshes.resize(1);
    density.resize(samples * samples * samples);
    for (size_t z = 0; z < samples; ++z) {
        for (size_t y = 0; y  < samples; ++y) {
            for (size_t x = 0; x < samples; ++x) {
                size_t idx = index(x, y, z);
                // density[idx] = static_cast<float>(y) - (SIZE / 2.0f);
                // density[idx] = static_cast<float>(y) - 1.5f;
                glm::vec3 p = glm::vec3(x,y,z);
                glm::vec3 center = glm::vec3(samples/2.0f);
                density[idx] = radius - glm::length(p-center);

            }
        }
    }
    printDensity();

    int cubesgenerated = 0;
    for (size_t z = 0; z < SIZE; ++z) {
        for (size_t y = 0; y  < SIZE; ++y) {
            for (size_t x = 0; x < SIZE; ++x) {
                uint8_t cubeindex = cubeIndex(x, y, z);
                std::println("cube ({}, {}, {}) -> cube index {}", x,y,z,cubeindex);

                int edgemask = edgeTable[cubeindex];
                if(edgemask == 0) {
                    continue;
                }
                ++cubesgenerated;

                for (int edge = 0; edge < 12; edge++) {
                    if (!(edgemask & (1 << edge))) {
                        continue;
                    }

                    int c0 = edgeCorners[edge][0];
                    int c1 = edgeCorners[edge][1];

                    glm::vec3 p0 = glm::vec3(
                            x + cornerOffsets[c0][0],
                            y + cornerOffsets[c0][1],
                            z + cornerOffsets[c0][2]
                    );
                    glm::vec3 p1 = glm::vec3(
                            x + cornerOffsets[c1][0],
                            y + cornerOffsets[c1][1],
                            z + cornerOffsets[c1][2]
                    );

                    edgelist[edge] = linearInterpolation(p0, p1, corners[c0], corners[c1]);
                 }


                const int* triangles = triTable[cubeindex];
                // int loopcounter = 0;
                for (int i = 0; triangles[i] != -1; i += 3) {
                    uint32_t index = marchingcubesdata.submeshes[0].vertices.size();
                    Lumos::Vertex vert;
                    vert.position = glm::vec3(edgelist[triangles[i]]);
                    marchingcubesdata.submeshes[0].vertices.push_back(vert);
                    // vertices.push_back(vert);
                    vert.position = glm::vec3(edgelist[triangles[i+1]]);
                    marchingcubesdata.submeshes[0].vertices.push_back(vert);
                    // vertices.push_back(vert);
                    vert.position = glm::vec3(edgelist[triangles[i+2]]);
                    // vertices.push_back(vert);
                    marchingcubesdata.submeshes[0].vertices.push_back(vert);

                    marchingcubesdata.submeshes[0].indices.push_back(index);
                    marchingcubesdata.submeshes[0].indices.push_back(index+1);
                    marchingcubesdata.submeshes[0].indices.push_back(index+2);
                }
            }
        }
    }
    std::println("cube generated: {}", cubesgenerated);
    std::println("size of data: {}", marchingcubesdata.submeshes[0].vertices.size());
    Lumos::AssetHandle handle = m_assetmanager->getMeshManager().load(marchingcubesdata);
    auto scene = m_scenemanager.getActiveScene();
    auto& ecs = scene->getEcs();
    auto ent = ecs.createEntity("world");
    Lumos::MeshComponent mesh;
    mesh.name = "world";
    mesh.assethandle = handle;
    ecs.addComponent<Lumos::MeshComponent>(ent, mesh);
    Lumos::Transform t;
    t.position = glm::vec3(0,0,0);
    ecs.addComponent<Lumos::Transform>(ent, t);
}

uint8_t Application::cubeIndex(size_t x, size_t y, size_t z)
{
    for (int i = 0; i < 8; ++i) {
        corners[i] = 0.0f;
    }
    uint8_t idx = 0;
    for (size_t i = 0; i < 8; i++) {
        int cx = x + cornerOffsets[i][0];
        int cy = y + cornerOffsets[i][1];
        int cz = z + cornerOffsets[i][2];
        corners[i] = density[index(cx, cy, cz)];
        if (corners[i] > 0) {
            idx |= (1 << i);
        }
    }
    return idx;
}
