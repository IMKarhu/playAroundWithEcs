#pragma once

class Renderer;
class Ecs;
class AssetManager;

class BasePass
{
public:
    BasePass();
    ~BasePass();

    void update(float dt, const Ecs &ecs, Renderer &renderer, const AssetManager& assetmanager);
private:
};
