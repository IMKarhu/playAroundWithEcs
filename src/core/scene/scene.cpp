#include "scene.h"
#include "assetmanagers/assetManager.h"
#include "ecsImpl/components.h"

namespace Lumos
{
    Scene::Scene(AssetManager& assetmanager, const std::string& name)
        :m_name(name)
    {
        // auto cube = m_ecs.createEntity("cube");
        // Lumos::AssetHandle sponza = assetmanager.getMeshManager().load("../src/game/assets/FlightHelmet.gltf");
        // MeshComponent mesh;
        // mesh.name = "world";
        // mesh.assethandle = sponza;
        // m_ecs.addComponent<MeshComponent>(cube, mesh);
        // m_ecs.addComponent<Transform>(cube, {});

        auto screen = m_ecs.createEntity("screen");
        ScreenQuad screenquad;
        screenquad.name = "screen";
        m_ecs.addComponent<ScreenQuad>(screen, screenquad);

        // auto camera = m_ecs.createEntity("camera");
        // m_ecs.addComponent<Transform>(camera, {.position = glm::vec3(0.0, 0.0, 10.0)});
        // m_ecs.addComponent<Camera>(camera, {.primary = true});
        //
        // auto light = m_ecs.createEntity("directional light");
        // m_ecs.addComponent<Transform>(light, {.position = glm::vec3(0.0, 10.0, 0.0)});
        // m_ecs.addComponent<Light>(light, {});
    }

    Entity Scene::createEntity(std::string name)
    {
        Entity ent = m_ecs.createEntity(name);
        return ent;
    }

    Ecs& Scene::getEcs()
    {
        return m_ecs;
    }
}// namespace Lumos
