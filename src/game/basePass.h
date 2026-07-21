#pragma once

class Ecs;
namespace Lumos
{
    class Renderer;
    class AssetManager;
}

class BasePass
{
public:
    BasePass();
    ~BasePass();

    void update(const Ecs &ecs, Lumos::Renderer &renderer, Lumos::AssetManager& assetmanager);
private:
};
