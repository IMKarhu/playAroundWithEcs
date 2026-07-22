#include "imageDecoder.h"
#include <print>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


namespace Lumos
{
    ImageDecoder::ImageDecoder() {}
    ImageDecoder::~ImageDecoder() {}
    ImageData ImageDecoder::decode(const TextureSource& src)
    {
        ImageData data;
        stbi_uc* pixels;
        size_t size;
        std::string fullpath = "../src/game/assets/"+src.path.string();
        std::println("full filepath: {}", fullpath);
        switch(src.type) {
            case TextureSource::Type::File:
                pixels = stbi_load(fullpath.c_str(), &data.width, &data.height, &data.channels, STBI_rgb_alpha);
                break;
            case TextureSource::Type::Byte:
                if(src.bytes.empty()) {
                    std::println("underlying data was destroyed");
                    break;
                }
                pixels = stbi_load_from_memory(src.bytes.data(),
                        static_cast<int>(src.bytes.size()),
                        &data.width, &data.height, &data.channels, STBI_rgb_alpha);
                break;
            default:
                std::println("Unknown texture type");
                break;
        }
        if (!pixels) {
            std::println("failed to load texture file");
            return data;
        }
        size = data.width * data.height * 4; //force to rgba
        data.pixels.assign(pixels, pixels + size);
        stbi_image_free(pixels);
        return data;
    }
}//namespace Lumos
