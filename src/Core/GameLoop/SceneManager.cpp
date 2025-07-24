#include "SceneManager.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/WorldSelectionScene.h"
#include "Scenes/GameScene.h"
#include "Utilities/Logger/Logger.h"
#include "Notifications/NotificationManager.h"

SceneManager::SceneManager(Renderer& renderer) : renderer(renderer)
{
    loadScene(SceneType::MainMenu);
}

void SceneManager::loadScene(SceneType sceneType, const std::string& worldName) // loading new scene and delete previous
{
    switch (sceneType) 
    {
        case SceneType::MainMenu:
            currentScene = std::make_unique<MainMenuScene>(renderer);
            break;

        case SceneType::WorldSelection:
            currentScene = std::make_unique<WorldSelectionScene>(renderer);
            break;

        case SceneType::Game:
            currentScene = std::make_unique<GameScene>(renderer, worldName);
            break;

        default:
            mycerr << "unknown scene";
            break;
    }
}

// dt = delta time for fps independency,    mouseVirtual = mouse position that was calculated in virtual screen
void SceneManager::update(float dt, Vector2 mouseVirtual)
{
    if (currentScene) 
    {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT); // reset mouse cursor to default
        currentScene->update(dt, mouseVirtual); // update scene
        NotificationManager::getInstance().update(dt); // update notifications

        if (currentScene->shouldTransition()) // scene changer
        {
            //        next scene                    name to load correct world
            loadScene(currentScene->getNextScene(), currentScene->getWorldName());
        }
    }
    else { mycerr << "no current scene"; }
}

void SceneManager::render() // draw everything
{
    if (currentScene)
    {
        BeginMode2D(renderer.GetCamera()); // draw everything in camera
        is2DModeDone = false; // reset camera mode flag 

        ClearBackground(BLACK); // clear screen
        currentScene->render(); // draw scene

        if (!is2DModeDone) EndMode2D();

        NotificationManager::getInstance().render(renderer); // draw notifications above everything
    }
    else { mycerr << "no current scene"; }
}