#pragma once

namespace Lumos
{
    enum class GraphicsApi
    {
        OPENGL,
        VULKAN
    };

    struct Platform
    {
        GraphicsApi api = GraphicsApi::OPENGL;
        bool isWayland = false;
    };
}// namespace Lumos

