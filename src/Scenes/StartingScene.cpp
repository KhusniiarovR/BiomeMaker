#include "StartingScene.h"
#include "raylib.h"
#include "Utilities/Input/InputAny.h"

StartingScene::StartingScene(Renderer& renderer) : Scene(renderer) {}

void StartingScene::update(float dt, Vector2 mouseVirtual) 
{
    updateChangeScene();
}

void StartingScene::render() const 
{
    renderer.drawBackground();
    renderer.drawTextGradient("Press any key to continue", {0.5f, 0.5f}, 40, 4.0f, PURPLE, RED);
}

void StartingScene::updateChangeScene() 
{
    if (inputAny())
    {
        changeScene = true;
        nextScene = SceneType::Menu;
    }
}

bool StartingScene::shouldTransition() const 
{
    return changeScene;
}

SceneType StartingScene::getNextScene() const 
{
    return nextScene;
}
