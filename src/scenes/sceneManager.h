#pragma once
#include <memory>
#include <string>
#include "scene.h"

class SceneManager
{
public:
    // this needs to be changed to filepath once I get to a point of implementing filepath stuff
    static std::shared_ptr<Scene> newScene(std::string name);

    static void play();
    static void stop();

    static std::shared_ptr<Scene> getActiveScene();
private:
    static std::shared_ptr<Scene> m_activeScene;
};
