#include "SceneManager.h"
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"
#include "Scenes/WorldSelectionScene.h"
#include "Scenes/WorldCreationScene.h"
#include "Utilities/Logger/Logger.h"

SceneManager::SceneManager(Renderer& renderer) : renderer(renderer) {
    loadScene(SceneType::MainMenu);
}

void SceneManager::loadScene(SceneType sceneType, const std::string& worldName) {
    switch (sceneType) {
        case SceneType::MainMenu:
            currentScene = std::make_unique<MainMenuScene>(renderer);
        break;
        case SceneType::Game:
            currentScene = std::make_unique<GameScene>(renderer, worldName);
        break;
        case SceneType::WorldSelection:
            currentScene = std::make_unique<WorldSelectionScene>(renderer);
        break;
        case SceneType::WorldCreation:
            currentScene = std::make_unique<WorldCreationScene>(renderer);
        break;
        default:
            mycerr << "unknown scene";
        break;
    }
}

void SceneManager::update(float dt, Vector2 mouseVirtual) {
    if (currentScene) {
        currentScene->update(dt, mouseVirtual);
        if (currentScene->shouldTransition()) {
            loadScene(currentScene->getNextScene(), currentScene->getWorldName());
        }
    }
    else {
        mycerr << "no current scene";
    }
}

void SceneManager::render() {
    if (currentScene) {
        BeginMode2D(renderer.GetCamera());
        ClearBackground(BLACK);
        currentScene->render();
    }
    else {
        mycerr << "no current scene";
    }
}