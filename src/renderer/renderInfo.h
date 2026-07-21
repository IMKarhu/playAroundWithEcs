#pragma once
#include <string>
#include <glm/glm.hpp>
#include "ecsImpl/components.h"
#include "assetmanagers/assetbase.h"

namespace Lumos
{
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
        glm::vec3 camerapos;
        Light lightdata;
        glm::vec3 lightpos;
        IMesh* mesh;
        AssetHandle screenpasscolorattachment;
        GBufferHandles attachments;
        std::string readframebuffer;
    };
}// namespace Lumos

