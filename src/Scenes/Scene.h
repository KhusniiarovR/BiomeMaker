#ifndef SCENE_H
#define SCENE_H

#include "Scenes/SceneType.h"
#include "Core/Render/Renderer.h"

class Scene {
protected:
    Renderer& renderer;

    bool changeScene = false;
    SceneType nextScene = SceneType::None;

public:
    Scene(Renderer& renderer) : renderer(renderer) {}
    virtual void update(float dt) = 0;
    virtual void render() const = 0;
    [[nodiscard]] virtual bool shouldTransition() const { return false; }
    [[nodiscard]] virtual SceneType getNextScene() const { return SceneType::None; }
    [[nodiscard]] virtual std::string getWorldName() const { return "";}

    virtual ~Scene() = default;
};

#endif //SCENE_H
