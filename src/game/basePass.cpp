#include "basePass.h"
#include "ecsImpl/ecs.h"
#include "ecsImpl/components.h"
#include "renderer.h"
#include "assetmanagers/assetManager.h"
#include <print>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

BasePass::BasePass()
{
}

BasePass::~BasePass() {}

void BasePass::update(const Ecs &ecs, Lumos::Renderer &renderer, Lumos::AssetManager& assetmanager)
{
    for (auto ent : ecs.view<Lumos::Transform, Lumos::MeshComponent>()) {
        auto& transform = ecs.getComponent<Lumos::Transform>(ent);
        auto& mesh = ecs.getComponent<Lumos::MeshComponent>(ent);
        Lumos::IMesh* rawmesh = assetmanager.getMeshManager().get(mesh.assethandle);
        if (!rawmesh) {
            continue;
        }
        Lumos::RenderInfo renderinfo;
        renderinfo.type = Lumos::InfoType::Geometry;
        renderinfo.shadername = "basePass";
        renderinfo.mesh = rawmesh;
        renderinfo.viewproj = transform.mvp;
        renderinfo.model = transform.model;
        renderer.submit(renderinfo);
    }
}
