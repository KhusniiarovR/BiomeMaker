#include "GameScene.h"
#include "raylib.h"

GameScene::GameScene(Renderer& renderer, const std::string& worldName) :
         Scene(renderer),
         player ({screenSizeX/2.0f, screenSizeY/2.0f}),
         world(worldName)
{
    renderer.GetCamera().offset = {screenSizeX/2.0f, screenSizeY/2.0f};
    // todo delete offset maybe
}

void GameScene::update(float dt) {
    player.update(dt);
    renderer.updateCamera(player.getPosition());
    world.update(player.getPosition());

    // todo function to handle scene manager inputs
    if (IsKeyPressed(KEY_SPACE)) {
        changeScene = true;
        nextScene = SceneType::MainMenu;
    }
}

void GameScene::render() const {
    world.render();
    renderer.drawPlayer(player.getPosition());
}

bool GameScene::shouldTransition() const {
    return changeScene;
    // todo pause
}

SceneType GameScene::getNextScene() const {
    return nextScene;
}
