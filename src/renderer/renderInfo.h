#pragma once
#include <string>
#include <glm/glm.hpp>

struct RenderInfo {
    std::string meshname;
    std::string shadername;
    glm::mat4 transform;
    uint32_t id;
};
