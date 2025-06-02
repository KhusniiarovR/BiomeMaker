#include "MainMenuScene.h"
#include <Constants/GraphicsConst.h>
#include <Utilities/graphics/Text.h>
#include "raylib.h"

void MainMenuScene::update(float dt) {
    if (IsKeyPressed(KEY_ENTER)) {
        changeScene = true;
    }
}

Text menu = {
    "Press ENTER to Start", {0.5f, 0.5f}, 100, RED
};

void MainMenuScene::draw() const {
    drawText(menu);
}
// todo drawText my implementation function

bool MainMenuScene::shouldTransition() const {
    return changeScene;
}

SceneType MainMenuScene::getNextScene() const {
    return SceneType::Game;
}
