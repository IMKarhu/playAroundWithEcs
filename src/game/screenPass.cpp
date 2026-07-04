#include "screenPass.h".h""
#include "ecsImpl/ecs.h"
#include "ecsImpl/components.h"
#include "renderer.h"
#include "assetManager.h"
#include "framebufferManager.h"
#include <print>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

ScreenPass::ScreenPass()
{
}

ScreenPass::~ScreenPass() {}

void ScreenPass::update(float dt,
                        const Ecs &ecs,
                        Renderer &renderer,
                        const AssetManager& assetmanager,
                        FrameBufferManager& framebuffermanager)
{
    for(auto ent : ecs.view<ScreenQuad>()) {
        auto& mesh = ecs.getComponent<ScreenQuad>(ent);
        const auto data = assetmanager.getModelAsset(mesh.name);
        for (const auto& handle : data.handles) {
            RenderInfo renderinfo;
            renderinfo.shadername = "screen";
            renderinfo.mesh = handle.mesh;
            renderinfo.texture = framebuffermanager.getFramebuffer("scene")->colorAttachment(0);
            renderer.submit(renderinfo);
        }
    }
}
