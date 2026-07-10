#pragma once
#include "assetbase.h"
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <string>

// struct Vertex
// {
//     glm::vec3 position;
//     std::array<float, 4> color = {1.0, 1.0, 1.0, 1.0};
//     std::array<float, 3> normal;
//     std::array<float, 2> texcoord;
// };

struct TextureHandle
{
    uint32_t id = 0;

    bool valid() const
    {
        return id != 0;
    }
};

struct MaterialResource
{
    TextureHandle basecolor;
    TextureHandle normal;
    TextureHandle metallicroughness;

    glm::vec4 basecolorfactor = glm::vec4(1);
    float metallicfactor = 1.0f;
    float roughnessfactor = 1.0f;

    bool doublesided = false;
};

struct MaterialHandle
{
    uint32_t id = 0;
};

struct MeshHandle
{
    uint32_t id = 0;
};

struct MeshComponent
{
    Lumos::AssetHandle assethandle;
    std::string name;
};

struct ScreenQuad
{
    std::string name;
    Lumos::AssetHandle assethandle;
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
    float fov = 45.0f;
    float nearclip = 0.1f;
    float farclip = 100.0f;
    float aspectratio = 1.777f;
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 proj = glm::mat4(1.0f);
    bool primary = false;
    glm::vec3 upvector = glm::vec3(0.0f, 1.0f, 0.0f);
};

struct Id
{
    std::string name;
    uint32_t id;
};

enum class LightType
{
    Directional,
    Point,
    Spot
};

struct Light
{
    LightType type = LightType::Directional;
    glm::vec3 color = glm::vec3(1);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
    float intensity = 1.0f;
    float radius = 1.0f;
};
