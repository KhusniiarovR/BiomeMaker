#include "WorldSelectionScene.h"
#include <thread>

WorldSelectionScene::WorldSelectionScene(Renderer &renderer)
    : Scene(renderer),
    playButton  ({0.6f, 0.3f},  {0.2f, 0.1f}, "PLAY",   BLACK, 0.5f),
    createButton({0.6f, 0.55f}, {0.2f, 0.1f}, "CREATE", BLACK, 0.5f),
    deleteButton({0.6f, 0.8f},  {0.2f, 0.1f}, "DELETE", BLACK, 0.5f)
{
    playButton.setOnClick([this]() {
        if (!worldSelector.getSelectedFolder().empty()) {
            changeScene = true;
            nextScene = SceneType::Game;
        }
    });

    createButton.setOnClick([this]() {
        changeScene = true;
        nextScene = SceneType::WorldCreation;
    });
    
    deleteButton.setOnClick([this]() {
        worldSelector.deleteCurrent();
    });
    
    playButton.setTexture(renderer.getTexture("button1"));
    createButton.setTexture(renderer.getTexture("button1"));
    deleteButton.setTexture(renderer.getTexture("button1"));
}

void WorldSelectionScene::update(float dt) {
    worldSelector.update();
    playButton.update();
    createButton.update();
    deleteButton.update();

    if (IsKeyPressed(KEY_ENTER)) {
        if (!worldSelector.getSelectedFolder().empty()) {
            changeScene = true;
            nextScene = SceneType::Game;
        }
    }

    if (IsKeyPressed(KEY_SPACE)) {
        changeScene = true;
        nextScene = SceneType::MainMenu;
    }
}

void WorldSelectionScene::render() const {
    renderer.drawBackground();
    worldSelector.render(renderer);
    playButton.render(renderer);
    createButton.render(renderer);
    deleteButton.render(renderer);
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
