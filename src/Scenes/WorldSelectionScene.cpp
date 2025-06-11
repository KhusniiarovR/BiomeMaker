#include "WorldSelectionScene.h"

WorldSelectionScene::WorldSelectionScene(Renderer &renderer) : Scene(renderer)  {
    worldCreator.Generate(123, "Hello World");
}

void WorldSelectionScene::update(float dt) {
    worldSelector.update();

    if (IsKeyPressed(KEY_ENTER)) {
        changeScene = true;
        nextScene = SceneType::Game;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        changeScene = true;
        nextScene = SceneType::MainMenu;
    }
}

void WorldSelectionScene::render() const {
    worldSelector.draw(renderer);
}

bool WorldSelectionScene::shouldTransition() const {
    return changeScene;
}

SceneType WorldSelectionScene::getNextScene() const {
    return nextScene;
}

std::string WorldSelectionScene::getWorldName() const {
    return worldSelector.getSelectedFolder();
}
