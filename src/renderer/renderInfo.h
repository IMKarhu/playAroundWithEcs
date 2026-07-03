#pragma once
#include <string>
#include <glm/glm.hpp>
#include "ecsImpl/components.h"

struct RenderInfo {
    std::string meshname;
    std::string shadername;
    glm::mat4 transform;
    MeshHandle mesh;
    TextureHandle texture;
    uint32_t vao;
    uint32_t indexCount;
    uint32_t texturehandle;
};
