#include "SceneManager.h"
#include <iostream>
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"

SceneManager::SceneManager(Renderer& renderer) : renderer(renderer) {
    loadScene(SceneType::MainMenu);
}

void SceneManager::loadScene(SceneType sceneType) {
    switch (sceneType) {
        case SceneType::MainMenu:
            currentScene = std::make_unique<MainMenuScene>(renderer);
        break;
        case SceneType::Game:
            currentScene = std::make_unique<GameScene>(renderer);
        break;
        default:
            std::cerr << "SceneManager/LoadScene unknown scene\n";
        break;
    }
}

void SceneManager::update(float dt) {
    if (currentScene) {
        currentScene->update(dt);
        if (currentScene->shouldTransition()) {
            loadScene(currentScene->getNextScene());
        }
    }
    else {
        std::cerr << "SceneManager/update no current scene\n";
    }
}

void SceneManager::render() {
    if (currentScene) {
        BeginDrawing();
        BeginMode2D(renderer.GetCamera());
        ClearBackground(BLACK);
        currentScene->render();
        EndDrawing();
    }
    else {
        std::cerr << "SceneManager/draw no current scene\n";
    }
}