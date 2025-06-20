#include "MainMenuScene.h"
#include <Constants/GraphicsConst.h>
#include "raylib.h"

MainMenuScene::MainMenuScene(Renderer& renderer) : Scene(renderer) {
    renderer.GetCamera().offset = {0, 0};
    renderer.GetCamera().target = {0, 0};
}

void MainMenuScene::update(float dt) {
    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        changeScene = true;
        nextScene = SceneType::WorldSelection;
    }
}

void MainMenuScene::render() const {
    renderer.drawTextGradient("Press enter to continue", {0.5f, 0.5f}, 100.0f, 4.0f, PURPLE, RED);
}

bool MainMenuScene::shouldTransition() const {
    return changeScene;
}

SceneType MainMenuScene::getNextScene() const {
    return nextScene;
}
