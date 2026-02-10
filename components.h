#include <glm/glm.hpp>
#include <array>
#include <vector>

struct Vertex
{
    std::array<float, 3> position;
    std::array<float, 3> color;
};

struct Vertices
{
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    uint32_t m_vao;
    uint32_t m_vbo;
    uint32_t m_ebo;
};

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Camera
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    glm::vec3 position;
};

struct Sprite
{
    float a;
};

