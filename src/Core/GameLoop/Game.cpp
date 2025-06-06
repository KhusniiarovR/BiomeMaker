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
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Soul Knight");
    screenSizeX = GetScreenWidth();
    screenSizeY = GetScreenHeight();
    SetTargetFPS(60);
    //ToggleFullscreen();


}

void Game::stop() {
    CloseWindow();
}

