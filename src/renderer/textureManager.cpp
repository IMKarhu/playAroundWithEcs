#include "textureManager.h"
#include "renderer.h"

TextureManager::TextureManager(const Renderer& renderer)
    :m_renderer(renderer)
{
}

TextureManager::~TextureManager() {}

TextureHandle TextureManager::load(std::filesystem::path path, TextureImportSettings settings)
{
    auto it = m_texturecache.find(path);
    if (it != m_texturecache.end()) {
        return it->second;
    }

    TextureHandle handle = m_renderer.createTexture(path.string(), settings);
    m_texturecache.emplace(path, handle);
    return handle;
}

