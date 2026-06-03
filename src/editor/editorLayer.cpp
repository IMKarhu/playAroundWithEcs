#include "editorLayer.h"
#include "windowing/window.h"
#include <print>

EditorLayer::EditorLayer(const Renderer &renderer, Window &window)
    : m_renderer(renderer), m_window(window), Layer("Editor Layer")
{
}

EditorLayer::~EditorLayer() {}

void EditorLayer::attach() {}

void EditorLayer::detach() {}

void EditorLayer::update(float dt) {}

void EditorLayer::event(Event &event) {}
