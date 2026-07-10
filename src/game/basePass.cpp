#include "basePass.h"
#include "ecsImpl/ecs.h"
#include "ecsImpl/components.h"
#include "renderer.h"
#include "assetManager.h"
#include <print>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

BasePass::BasePass()
{
}

BasePass::~BasePass() {}

void BasePass::update(const Ecs &ecs, Renderer &renderer, Lumos::AssetManager& assetmanager)
{
    for (auto ent : ecs.view<Transform, MeshComponent>()) {
        auto& transform = ecs.getComponent<Transform>(ent);
        auto& mesh = ecs.getComponent<MeshComponent>(ent);
        Lumos::IMesh* rawmesh = assetmanager.getMeshManager().get(mesh.assethandle);
        if (!rawmesh) {
            continue;
        }
        RenderInfo renderinfo;
        renderinfo.type = InfoType::Geometry;
        renderinfo.shadername = "basePass";
        renderinfo.mesh = rawmesh;
        renderinfo.viewproj = transform.mvp;
        renderinfo.model = transform.model;
        renderer.submit(renderinfo);
    }
}
