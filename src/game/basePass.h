#pragma once

class Renderer;
class Ecs;
namespace Lumos
{
    class AssetManager;
}

class BasePass
{
public:
    BasePass();
    ~BasePass();

    void update(const Ecs &ecs, Renderer &renderer, Lumos::AssetManager& assetmanager);
private:
};
