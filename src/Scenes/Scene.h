#ifndef SCENE_H
#define SCENE_H

#include "Scenes/SceneType.h"

class Scene {
public:
    virtual void update(float dt) = 0;
    virtual void draw() const = 0;
    [[nodiscard]] virtual bool shouldTransition() const { return false; }
    [[nodiscard]] virtual SceneType getNextScene() const { return SceneType::None; }

    virtual ~Scene() = default;
};

#endif //SCENE_H
