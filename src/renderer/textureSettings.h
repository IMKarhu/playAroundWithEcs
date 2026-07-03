#pragma once

enum class TextureColorSpace
{
    Linear,
    SRGB
};

struct TextureImportSettings
{
    TextureColorSpace colorspace = TextureColorSpace::SRGB;
    bool generatemipmaps = true;
};
