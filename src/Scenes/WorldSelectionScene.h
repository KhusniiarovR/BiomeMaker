#ifndef WORLDSELECTIONSCENE_H
#define WORLDSELECTIONSCENE_H

#include "Scene.h"
#include <Core/World/WorldSelector.h>
#include "Core/World/WorldCreator.h"

class WorldSelectionScene : public Scene {
private:
    WorldCreator worldCreator;
    WorldSelector worldSelector;

public:
    WorldSelectionScene(Renderer& renderer);

    void update(float dt) override;
    void render() const override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
    [[nodiscard]] std::string getWorldName() const override;
};

#endif //WORLDSELECTIONSCENE_H
