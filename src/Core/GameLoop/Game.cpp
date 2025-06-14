#include "Game.h"
#include "raylib.h"
#include "Constants/GraphicsConst.h"

Game::Game() : renderer(assetManager),
          sceneManager(renderer) {
    init();
}

void Game::run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        sceneManager.update(dt);
        sceneManager.render();
    }
    stop();
}

void Game::init() {
    SetTraceLogLevel(LOG_ERROR); // opengl init comments
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Biome Maker");
    screenSizeX = GetScreenWidth();
    screenSizeY = GetScreenHeight();
    SetTargetFPS(60);
    SetMouseOffset(0, 28);
    //ToggleFullscreen();
}

void Game::stop() {
    CloseWindow();
}

