#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <string>

struct Vertex
{
    std::array<float, 3> position;
    std::array<float, 4> color = {1.0, 1.0, 1.0, 1.0};
    std::array<float, 3> normal;
    std::array<float, 2> texcoord;
};


struct Texture
{
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t channels = 0;
    std::vector<unsigned char> pixels;
};

struct TextureHandle
{
    uint32_t id = 0;

    bool valid() const
    {
        return id != 0;
    }
};

struct MeshResource
{
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t indexCount;
};

struct MeshHandle
{
    uint32_t id = 0;
};

struct SubMesh {
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t vbo = 0;
    uint32_t ebo = 0;
};

struct Mesh
{
    std::vector<SubMesh> submeshes;
    std::vector<Texture> textures;
    uint32_t id;
    std::string name;
};

struct ScreenQuad
{
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    uint32_t vbo = 0;
    uint32_t ebo = 0;

};

struct Transform
{
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 rotation = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::mat4 model = glm::mat4(1.0f);
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

struct InputBindigs
{
};

