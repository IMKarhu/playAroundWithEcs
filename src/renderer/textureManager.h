#pragma once
#include <unordered_map>
#include <filesystem>

#include "ecsImpl/components.h"
#include "textureSettings.h"

class Renderer;

class TextureManager
{
public:
    TextureManager(const Renderer& renderer);
    ~TextureManager();

    TextureHandle load(std::filesystem::path path, TextureImportSettings settings);
private:
    std::unordered_map<std::filesystem::path, TextureHandle> m_texturecache;
    const Renderer& m_renderer;
};
