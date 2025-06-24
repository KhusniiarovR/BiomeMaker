#include "GameScene.h"
#include "raylib.h"
#include <Constants/GraphicsConst.h>

GameScene::GameScene(Renderer& renderer, const std::string& worldName) :
         Scene(renderer),
         player ({worldSize * tileSize / 2.0f, worldSize * tileSize / 2.0f}),
         enemy ({worldSize * tileSize / 2.0f, worldSize * tileSize / 2.0f}, player),
         world(worldName)
{
    renderer.GetCamera().offset = {screenSizeX / 2.0f, screenSizeY / 2.0f};
    // TODO add zoom and delete offset
}

void GameScene::update(float dt) {
    player.update(dt);
    enemy.update(dt);
    renderer.updateCamera(player.getPosition());
    world.update(player.getPosition(), renderer.GetCamera());

    // TODO function to handle scene manager inputs
    if (IsKeyPressed(KEY_SPACE)) {
        changeScene = true;
        nextScene = SceneType::MainMenu;
    }
}

void GameScene::render() const {
    world.render(renderer);
    renderer.drawPlayer(player.getPosition());
    renderer.drawEnemy(enemy.getPosition());
}

bool GameScene::shouldTransition() const {
    return changeScene;
    // TODO pause
}

SceneType GameScene::getNextScene() const {
    return nextScene;
}
