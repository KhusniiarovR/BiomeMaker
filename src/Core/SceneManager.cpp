#include "SceneManager.h"
#include <iostream>
#include "Scenes/MainMenuScene.h"
#include "Scenes/GameScene.h"

SceneManager::SceneManager() {
    loadScene(SceneType::MainMenu);
}

void SceneManager::loadScene(SceneType sceneType) {
    switch (sceneType) {
        case SceneType::MainMenu:
            currentScene = std::make_unique<MainMenuScene>();
        break;
        case SceneType::Game:
            currentScene = std::make_unique<GameScene>();
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

void SceneManager::draw() {
    if (currentScene) {
        BeginDrawing();
        ClearBackground(BLACK);
        currentScene->draw();
        EndDrawing();
    }
    else {
        std::cerr << "SceneManager/draw no current scene\n";
    }
}