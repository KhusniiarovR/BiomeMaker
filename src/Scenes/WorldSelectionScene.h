#ifndef WORLDSELECTIONSCENE_H
#define WORLDSELECTIONSCENE_H

#include "Scene.h"
#include "World/WorldSelector.h"
#include "World/WorldCreator.h"
#include "Utilities/Graphics/Button.h"

class WorldSelectionScene : public Scene {
private:
    WorldCreator worldCreator;
    WorldSelector worldSelector;

    Button playButton;
    Button createButton;
    Button deleteButton;

public:
    explicit WorldSelectionScene(Renderer& renderer);

    void update(float dt) override;
    void render() const override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
    [[nodiscard]] std::string getWorldName() const override;
};

#endif //WORLDSELECTIONSCENE_H
