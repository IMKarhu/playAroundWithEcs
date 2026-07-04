#pragma once
#include <string>
#include <glm/glm.hpp>
#include "ecsImpl/components.h"

struct RenderInfo {
    std::string shadername;
    glm::mat4 transform;
    MeshHandle mesh;
    TextureHandle texture;
};
