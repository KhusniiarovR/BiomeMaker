#include "MenuScene.h"
#include "raylib.h"
#include "Utilities/Input/InputAny.h"
#include "Constants/GraphicsConst.h"

MenuScene::MenuScene(Renderer& renderer) 
: Scene(renderer),
playButton    ({0.25f, 0.35f}, {0.5f, 0.15f}, "play",     BLACK, 0.5f),
settingsButton({0.25f, 0.55f}, {0.5f, 0.15f}, "settings", BLACK, 0.5f),
quitButton    ({0.25f, 0.75f}, {0.5f, 0.15f}, "quit",     BLACK, 0.5f)
{
    // return camera to {0,0}, because game moves it to the center
    renderer.GetCamera().offset = {0, 0}; 
    renderer.GetCamera().target = {0, 0};

    // buttons functional
    playButton.setOnClick([this]() { changeScene = true;nextScene = SceneType::WorldSelection;});
    settingsButton.setOnClick([this]() { settings.setActive(!settings.isActive()); });
    quitButton.setOnClick([this]() { shouldExit = true; });

    // button textures
    playButton.setTexture(renderer.getTexture("button1")); 
    settingsButton.setTexture(renderer.getTexture("button1"));
    quitButton.setTexture(renderer.getTexture("button1"));
}

void MenuScene::update(float dt, Vector2 mouseVirtual) 
{
    // settings
    if (settings.isActive()) {
        settings.update(dt, mouseVirtual);
        return; 
    }
    
    // buttons
    playButton.update(mouseVirtual); 
    settingsButton.update(mouseVirtual);
    quitButton.update(mouseVirtual);

    updateChangeScene();
}

void MenuScene::render() const 
{
    // parallax background 
    renderer.drawBackground();

    // settings
    if (settings.isActive()) {
        settings.render(renderer);
        return; 
    }

    // big game name text
    renderer.drawTextGradient("Biome Maker", {0.5f, 0.15f}, 60, 4.0f, PURPLE, RED);
    
    // buttons
    playButton.render(renderer); 
    settingsButton.render(renderer);
    quitButton.render(renderer);
}

void MenuScene::updateChangeScene() 
{
    if (InputManager::GetInstance().IsActionPressed(Action::ESCAPE)) { shouldExit = true; }
}

bool MenuScene::shouldTransition() const 
{
    return changeScene;
}

SceneType MenuScene::getNextScene() const 
{
    return nextScene;
}