#include <array>
#include <vector>

struct Vertex
{
    std::array<float, 3> position;
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
    std::array<float, 3> m_position;
    std::array<float, 3> m_rotation;
    std::array<float, 3> m_scale;
};

struct Sprite
{
    float a;
};

