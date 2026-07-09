#pragma once
#include <string>

#include <glm/glm.hpp>

struct RenderPassDesc
{
    std::string name;
    std::string framebuffer;
    glm::vec4 clearcolor = glm::vec4(0, 0, 0, 1);

    bool clearcolorbuffer = true;
    bool cleardepthbuffer = true;
    bool depthtest = true;
};
