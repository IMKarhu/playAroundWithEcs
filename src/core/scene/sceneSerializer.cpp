#include "sceneSerializer.h"
#include "assetmanagers/assetManager.h"
#include "json.hpp"
#include <fstream>
#include <print>

namespace Lumos
{
    SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
        :m_scene(scene) {}

    bool SceneSerializer::serialize(const std::string& filepath)
    {
        //TODO: write to json/binary file
        return true;
    }

    //expand whenever new components gets introduced
    //also should handle binary files
    bool SceneSerializer::deSerialize(const std::string& filepath, AssetManager& assetmanager)
    {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::println("failed to load file");
            return false;
        }

        Ecs& ecs = m_scene->getEcs();

        nlohmann::json scenedata;
        file >> scenedata;
        for (const auto& jsonent : scenedata["Entities"]) {
            std::string name = jsonent.value("Name", "Unnamed Entity");

            Entity entity = m_scene->createEntity(name);
            if (jsonent.contains("TransformComponent")) {
                Transform t;
                auto positiondata = jsonent["TransformComponent"]["Position"];
                t.position = glm::vec3(positiondata[0], positiondata[1], positiondata[2]);
                ecs.addComponent<Transform>(entity, t);
            }
            if (jsonent.contains("MeshComponent")) {
                std::string path = jsonent["MeshComponent"]["FilePath"];
                AssetHandle handle = assetmanager.getMeshManager().load(path);
                MeshComponent mesh;
                mesh.name = "world";
                mesh.assethandle = handle;
                ecs.addComponent<MeshComponent>(entity, mesh);
            }
            if (jsonent.contains("CameraComponent")) {
                auto fov = jsonent["CameraComponent"]["Fov"];
                auto far = jsonent["CameraComponent"]["Far"];
                auto near = jsonent["CameraComponent"]["Near"];
                auto ratio = jsonent["CameraComponent"]["Aspectratio"];
                auto view = jsonent["CameraComponent"]["View"];
                auto proj = jsonent["CameraComponent"]["Projection"];
                auto primary = jsonent["CameraComponent"]["Primary"];
                auto up = jsonent["CameraComponent"]["Up"];
                Camera cam = {
                    .fov = fov,
                    .nearclip = near,
                    .farclip = far,
                    .aspectratio = ratio,
                    .view = glm::mat4(view),
                    .proj = glm::mat4(proj),
                    .primary = primary,
                    .upvector = glm::vec3(up[0], up[1], up[2])
                };
                ecs.addComponent<Camera>(entity, cam);
            }
            if (jsonent.contains("LightComponent")) {
                auto type = jsonent["LightComponent"]["LightType"];
                auto color = jsonent["LightComponent"]["Color"];
                auto dir = jsonent["LightComponent"]["Direction"];
                auto intensity = jsonent["LightComponent"]["Intensity"];
                auto radius = jsonent["LightComponent"]["Radius"];
                Light light = {
                    .type = type,
                    .color = glm::vec3(color[0], color[1], color[2]),
                    .direction = glm::vec3(dir[0], dir[1], dir[2]),
                    .intensity = intensity,
                    .radius = radius
                };
                ecs.addComponent<Light>(entity, light);
            }
        }
        file.close();
        return true;
    }
}// namespace Lumos
