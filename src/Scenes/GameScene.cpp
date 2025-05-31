#include "GameScene.h"
#include "raylib.h"

GameScene::GameScene() : player ({GetScreenWidth()/2.0f, GetScreenHeight()/2.0f}) {}

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
