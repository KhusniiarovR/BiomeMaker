#include "MainMenuScene.h"
#include "raylib.h"

void MainMenuScene::update(float dt) {
    if (IsKeyPressed(KEY_ENTER)) {
        changeScene = true;
    }
}

void MainMenuScene::draw() const {
    DrawText("Press ENTER to Start", GetScreenWidth()/2, GetScreenHeight()/2, 100, RED);
}
// todo drawText my implementation function

bool MainMenuScene::shouldTransition() const {
    return changeScene;
}

SceneType MainMenuScene::getNextScene() const {
    return SceneType::Game;
}
