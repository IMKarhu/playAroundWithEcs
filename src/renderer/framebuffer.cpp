#include "framebuffer.h"
#include "opengl/glframebuffer.h"

namespace Lumos
{
    std::shared_ptr<FrameBuffer> FrameBuffer::create(const FramebufferSpec& spec)
    {
        return std::make_shared<GLFramebuffer>(spec);
    }
}// namespace Lumos

