#include "GameScene.h"
#include "raylib.h"

GameScene::GameScene(Renderer& renderer) : Scene(renderer), player ({screenSizeX/2.0f, screenSizeY/2.0f}) {
    renderer.GetCamera().offset = {screenSizeX/2.0f, screenSizeY/2.0f};
    // todo delete offset maybe
}

void GameScene::update(float dt) {
    player.update(dt);
    renderer.updateCamera(player.getPosition());
    world.update(player.getPosition());
}

void GameScene::render() const {
    world.render();
    renderer.drawPlayer(player.getPosition());
}

bool GameScene::shouldTransition() const {
    return false;
    // todo pause and lose
}

SceneType GameScene::getNextScene() const {
    return SceneType::None;
}
