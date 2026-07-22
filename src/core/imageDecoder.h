#pragma once
#include "assetmanagers/assetbase.hpp"

namespace Lumos
{
    // struct ImageData
    // {
    //     int width;
    //     int height;
    //     int channels;
    //     unsigned char* pixels;
    // };
    class ImageDecoder
    {
    public:
        ImageDecoder();
        ~ImageDecoder();

        ImageData decode(const TextureSource& src);
    private:
    };
}//namespace Lumos
