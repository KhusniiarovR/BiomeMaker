#include "GameScene.h"
#include <Constants/GraphicsConst.h>
#include "raylib.h"

GameScene::GameScene() : player ({screenSizeX/2.0f, screenSizeY/2.0f}) {}

void GameScene::update(float dt) {
    player.update(dt);
}

void GameScene::draw() const {
    player.draw();
}

bool GameScene::shouldTransition() const {
    return false;
    // todo pause and lose
}

SceneType GameScene::getNextScene() const {
    return SceneType::None;
}
