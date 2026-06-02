#pragma once
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <string>

struct Vertex
{
    std::array<float, 3> position;
    std::array<float, 3> color;
};

struct Mesh
{
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    // uint32_t vao = 0;
    uint32_t vbo = 0;
    uint32_t ebo = 0;
};

struct Transform
{
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

struct Camera
{
    glm::mat4 view;
    glm::mat4 proj;
};

struct model
{
    glm::mat4 model;
};

struct Sprite
{
    float a;
};

struct Id
{
    std::string name;
    uint32_t id;
};

// struct DefaultCube
// {
//     DefaultCube() {
//         vertices = {
//             // front
//             {{-0.5f, -0.5f,  0.5f}, {1, 0, 0}},
//             {{ 0.5f, -0.5f,  0.5f}, {0, 1, 0}},
//             {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}},
//             {{-0.5f,  0.5f,  0.5f}, {1, 1, 1}},
//             // back
//             {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}},
//             {{-0.5f, -0.5f, -0.5f}, {0, 1, 0}},
//             {{-0.5f,  0.5f, -0.5f}, {0, 0, 1}},
//             {{ 0.5f,  0.5f, -0.5f}, {1, 1, 1}},
//             // left
//             {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
//             {{-0.5f, -0.5f,  0.5f}, {0, 1, 0}},
//             {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}},
//             {{-0.5f,  0.5f, -0.5f}, {1, 1, 1}},
//             // right
//             {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}},
//             {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}},
//             {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}},
//             {{ 0.5f,  0.5f,  0.5f}, {1, 1, 1}},
//             // top
//             {{-0.5f,  0.5f,  0.5f}, {1, 0, 0}},
//             {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}},
//             {{ 0.5f,  0.5f, -0.5f}, {0, 0, 1}},
//             {{-0.5f,  0.5f, -0.5f}, {1, 1, 1}},
//             // bottom
//             {{-0.5f, -0.5f, -0.5f}, {1, 0, 0}},
//             {{ 0.5f, -0.5f, -0.5f}, {0, 1, 0}},
//             {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}},
//             {{-0.5f, -0.5f,  0.5f}, {1, 1, 1}},
//         };
//
//         indices = {
//             0, 1, 2, 2, 3, 0,
//             4, 5, 6, 6, 7, 4,
//             8, 9, 10, 10, 11, 8,
//             12, 13, 14, 14, 15, 12,
//             16, 17, 18, 18, 19, 16,
//             20, 21, 22, 22, 23, 20
//         };
//
//         glCreateVertexArrays(1, &vao);
//         glCreateBuffers(1, &vbo);
//         glCreateBuffers(1, &ebo);
//
//         glNamedBufferData(vbo,
//                 vertices.size() * sizeof(Vertex),
//                 vertices.data(),
//                 GL_STATIC_DRAW
//         );
//
//         glNamedBufferData(ebo,
//                 indices.size() * sizeof(uint32_t),
//                 indices.data(),
//                 GL_STATIC_DRAW
//         );
//
//         glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
//         glVertexArrayElementBuffer(vao, ebo);
//
//         glEnableVertexArrayAttrib(vao, 0);
//         glVertexArrayAttribFormat(vao,
//                 0,
//                 3,
//                 GL_FLOAT,
//                 GL_FALSE,
//                 offsetof(Vertex, position)
//         );
//         glVertexArrayAttribBinding(vao, 0, 0);
//
//         glEnableVertexArrayAttrib(vao, 1);
//         glVertexArrayAttribFormat(vao,
//                 1,
//                 3,
//                 GL_FLOAT,
//                 GL_FALSE,
//                 offsetof(Vertex, color)
//         );
//         glVertexArrayAttribBinding(vao, 1, 0);
//     }
//     std::vector<Vertex> vertices;
//     std::vector<uint32_t> indices;
//     uint32_t vao = 0;
//     uint32_t vbo = 0;
//     uint32_t ebo = 0;
// };

