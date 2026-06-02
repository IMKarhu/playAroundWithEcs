#pragma once
#include <cstdint>
#include <vector>
#include <memory>

enum class DepthFormatType {
    Texture,
    Buffer
};

enum class DepthFormat {
    None,
    Depth32F,
    Depth24Stencil8
};

//color formats for texture
enum class Format {
    RGBA8
};

struct FramebufferAttachmentSpec
{
    uint32_t texture;
    Format format;
};

struct FramebufferSpec
{
    uint32_t width;
    uint32_t height;
    uint32_t fbo;
    uint32_t depthAttachment;
    uint32_t rbo;
    std::vector<FramebufferAttachmentSpec> attachments;
    uint32_t samples = 1;
    DepthFormat depthFormat = DepthFormat::None;
    DepthFormatType depthType = DepthFormatType::Buffer;
};


class FrameBuffer
{
public:
    virtual ~FrameBuffer() = default;

    //I am not really a fan of having these bind and unbind functions.. but it works for now.
    //framebufferManager handles binding and ubinding so I guess having these are ok.
    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;
    static std::shared_ptr<FrameBuffer> create(const FramebufferSpec& spec);

    virtual uint32_t colorAttachment() const = 0;
    virtual FramebufferSpec &framebufferSpec() = 0;
private:
};

