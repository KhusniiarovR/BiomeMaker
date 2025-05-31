#include "Game.h"
#include "raylib.h"
#include <iostream>
#include <Scenes/MainMenuScene.h>
#include <Scenes/GameScene.h>

Game::Game() {
    init();
    loadScene(SceneType::MainMenu);
}

Game::~Game() = default;

void Game::run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        update(dt);

        BeginDrawing();
        draw();
        EndDrawing();
    }
    stop();
}

void Game::loadScene(SceneType sceneType) {
    switch (sceneType) {
        case SceneType::MainMenu:
            currentScene = std::make_unique<MainMenuScene>();
            break;
        case SceneType::Game:
            currentScene = std::make_unique<GameScene>();
            break;
        default:
            std::cerr << "Game/Load Scene unknown scene\n";
            break;
    }
}


void Game::init() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Soul Knight");
    SetTargetFPS(60);
    //ToggleFullscreen();
}

void Game::stop() {
    CloseWindow();
}

void Game::update(float dt) {
    if (currentScene) {
        currentScene->update(dt);
        if (currentScene->shouldTransition()) {
            loadScene(currentScene->getNextScene());
        }
    }
    else {
        std::cerr << "Game/update no current scene";
    }
}

void Game::draw() {
    if (currentScene) {
        ClearBackground(BLACK);
        currentScene->draw();
    }
    else {
        std::cerr << "Game/draw no current scene";
    }
}