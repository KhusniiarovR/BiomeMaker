#include "MainMenuScene.h"
#include <Constants/GraphicsConst.h>
#include "raylib.h"

MainMenuScene::MainMenuScene(Renderer& renderer) : Scene(renderer) {
    renderer.GetCamera().offset = {0, 0};
}
// todo create separate scene for world creation/selection

void MainMenuScene::update(float dt) {
    if (IsKeyPressed(KEY_ENTER)) {
        changeScene = true;
        nextScene = SceneType::WorldSelection;
    }
}

void MainMenuScene::render() const {
    renderer.drawText("Press enter to continue");
}

bool MainMenuScene::shouldTransition() const {
    return changeScene;
}

SceneType MainMenuScene::getNextScene() const {
    return nextScene;
}
