#pragma once
#include "layer.h"
#include "windowing/window.h"
#include "renderer.h"
#include "assetmanagers/assetManager.h"
#include "scene/sceneManager.h"
#include "windowing/events.h"
#include <vector>
#include <memory>

class Application
{
public:
    Application(Lumos::Platform platform);
    void run();
    void pushLayer(std::unique_ptr<Lumos::Layer> layer);
    void event(Lumos::Event& event);
private:
    std::vector<std::unique_ptr<Lumos::Layer>> m_layers;
    std::unique_ptr<Lumos::Window> m_window;
    std::shared_ptr<Lumos::Renderer> m_renderer;
    std::unique_ptr<Lumos::AssetManager> m_assetmanager;
    Lumos::SceneManager m_scenemanager;
    std::string m_titleAndfps;

    //testing for world generation

    //number of "cubes" ie a chunk would contain 4x4x4 cubes
    static constexpr uint32_t SIZE = 32;
    static constexpr uint32_t samples = SIZE + 1;
    float corners[8];
    glm::vec3 edgelist[12];

    std::vector<float> density;
    std::vector<Lumos::Vertex> vertices;
    std::vector<uint32_t> indices;
    Lumos::ModelData marchingcubesdata;
    std::shared_ptr<Lumos::IMesh> testmesh;

    size_t index(uint32_t x, uint32_t y, uint32_t z);
    glm::vec3 linearInterpolation(const glm::vec3& corner1, const glm::vec3& corner2, float density0, float density1);
    void printDensity();
    void marchingCubes();
    uint8_t cubeIndex(size_t x, size_t y, size_t z);
};
