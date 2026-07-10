#pragma once
#include <string>
#include <memory>
#include "windowing/events.h"

class Renderer;
namespace Lumos
{
    class Scene;
    class Layer
    {
        public:
            Layer(const std::string &name = "defaultLayer")
                : m_layerName(name) {}
            virtual ~Layer() = default;
            virtual void attach() {}
            virtual void detach() {}
            virtual void update(float dt, const std::shared_ptr<Lumos::Scene>& scene) {}
            virtual void editorUpdate() {}
            virtual void event(Event &event, const std::shared_ptr<Lumos::Scene>& scene) {}
        private:
            std::string m_layerName;
    };
}

