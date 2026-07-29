#pragma once
#include "assetmanagers/assetbase.hpp"
#include <glm/glm.hpp>
#include <string>

namespace Lumos
{
    struct TextureHandle
    {
        uint32_t id = 0;

        bool valid() const
        {
            return id != 0;
        }
    };

    struct MeshHandle
    {
        uint32_t id = 0;
    };

    struct MeshComponent
    {
        std::string name;
        AssetHandle assethandle;
    };

    struct ScreenQuad
    {
        std::string name;
        AssetHandle assethandle;
    };

    struct Transform
    {
        glm::vec3 position = glm::vec3(0, 0, 0);
        glm::vec3 rotation = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1, 1, 1);
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 mvp = glm::mat4(1.0f);
    };

    struct Camera
    {
        glm::vec3 upvector = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        float fov = 45.0f;
        float nearclip = 0.1f;
        float farclip = 100.0f;
        float aspectratio = 1.777f;
        bool primary = false;
    };

    struct Id
    {
        std::string name;
        uint32_t id;
    };

    enum class LightType
    {
        Directional = 0,
        Point = 1,
        Spot = 2
    };

    struct Light
    {
        LightType type = LightType::Directional;
        glm::vec3 color = glm::vec3(1);
        glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
        float intensity = 1.0f;
        float radius = 1.0f;
    };

    struct Movement
    {
        bool moved = false;
        float speed = 1.0f;
        int button = -1;
    };
}// namespace Lumos
