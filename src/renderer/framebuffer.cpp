#include "framebuffer.h"
#include "opengl/glframebuffer.h"

std::shared_ptr<FrameBuffer> FrameBuffer::create(const FramebufferSpec& spec)
{
    return std::make_shared<GLFramebuffer>(spec);
}
