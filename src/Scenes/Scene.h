#pragma once
#include "Scenes/SceneType.h"
#include "Core/Render/Renderer.h"

class Scene {
protected:
    Renderer& renderer; // to draw everything in scenes

    bool changeScene = false; // change scene logic
    SceneType nextScene = SceneType::None; // change scene logic

public:
    Scene(Renderer& renderer) : renderer(renderer) {}
    virtual ~Scene() = default;
    virtual void update(float dt, Vector2 mouseVirtual) = 0;
    virtual void render() const = 0;

    // scene transition logic
    virtual void updateChangeScene() = 0;
    [[nodiscard]] virtual bool shouldTransition() const { return false; }
    [[nodiscard]] virtual SceneType getNextScene() const { return SceneType::None; }
    [[nodiscard]] virtual std::string getWorldName() const { return "";}
};