#pragma once

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
