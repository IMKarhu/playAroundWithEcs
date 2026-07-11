#include "screenPass.h"
#include "ecsImpl/ecs.h"
#include "ecsImpl/components.h"
#include "renderer.h"
#include <print>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

ScreenPass::ScreenPass()
{
}

ScreenPass::~ScreenPass() {}

void ScreenPass::update(const Ecs &ecs,
                        Lumos::Renderer &renderer,
                        uint32_t framebuffercolorattachment)
{
    for(auto ent : ecs.view<Lumos::ScreenQuad>()) {
        auto& quad = ecs.getComponent<Lumos::ScreenQuad>(ent);
        Lumos::RenderInfo info;
        info.type = Lumos::InfoType::Screen;
        info.readframebuffer = "yes";
        info.shadername = "screen";
        info.screenpasscolorattachment.id = framebuffercolorattachment;
        info.mesh = nullptr;
        renderer.submit(info);
    }
}
