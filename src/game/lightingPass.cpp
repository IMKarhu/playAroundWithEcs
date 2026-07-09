#include "lightingPass.h"
#include "ecsImpl/ecs.h"
#include "ecsImpl/components.h"
#include "renderer.h"
#include "framebufferManager.h"
#include <print>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


LightingPass::LightingPass()
{
}

LightingPass::~LightingPass()
{
}

void LightingPass::update(const Ecs &ecs, Renderer &renderer, FrameBufferManager& framebuffermanager)
{
    for (auto ent : ecs.view<Transform, Light>()) {
        auto& transform = ecs.getComponent<Transform>(ent);
        auto& light = ecs.getComponent<Light>(ent);
        auto gbuffer = framebuffermanager.getFramebuffer("gbuffer");
        RenderInfo info;
        info.type = InfoType::Lighting;
        info.shadername = "lighting";
        info.attachments.attachment0.id = gbuffer->colorAttachment(0).id;
        info.attachments.attachment1.id = gbuffer->colorAttachment(1).id;
        info.attachments.attachment2.id = gbuffer->colorAttachment(2).id;
        info.lightcolor = light.color;
        info.lightdir = light.direction;
        renderer.submit(info);
    }
}

