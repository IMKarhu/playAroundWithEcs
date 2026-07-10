#pragma once
#include <string>
#include <glm/glm.hpp>
#include "assetmanagers/assetbase.h"

enum class InfoType
{
    Geometry,
    Lighting,
    Screen
};

struct RenderInfo {
    InfoType type;
    std::string shadername;
    glm::mat4 viewproj;
    glm::mat4 model;
    glm::vec3 lightdir;
    glm::vec3 lightcolor;
    Lumos::IMesh* mesh;
    Lumos::AssetHandle screenpasscolorattachment;
    Lumos::GBufferHandles attachments;
    std::string readframebuffer;
};
