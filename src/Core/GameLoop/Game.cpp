#include "Game.h"
#include "raylib.h"
#include "Constants/GraphicsConst.h"

Game::Game() : renderer(assetManager, virtualScreenSizeX, virtualScreenSizeY, mouseVirtual), sceneManager(renderer) {
    init();
}

void Game::run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        mouseVirtual = getMouseVirtual();

        sceneManager.update(dt, mouseVirtual);

        BeginTextureMode(virtualScreen);
        ClearBackground(BLACK);
        sceneManager.render();
        EndTextureMode();
    
        BeginDrawing();
        ClearBackground(BLACK);

        float scaleX = (float)GetScreenWidth() / virtualScreenSizeX;
        float scaleY = (float)GetScreenHeight() / virtualScreenSizeY;
        renderScale = fmin(scaleX, scaleY);

        int scaledWidth = (int)(virtualScreenSizeX * renderScale);
        int scaledHeight = (int)(virtualScreenSizeY * renderScale);
        offsetX = (GetScreenWidth() - scaledWidth) / 2;
        offsetY = (GetScreenHeight() - scaledHeight) / 2;

        Rectangle src = { 0, 0, (float)virtualScreenSizeX, -(float)virtualScreenSizeY };
        Rectangle dst = { (float)offsetX, (float)offsetY, (float)scaledWidth, (float)scaledHeight };

        DrawTexturePro(virtualScreen.texture, src, dst, { 0, 0 }, 0.0f, WHITE);

        EndDrawing();
    }

    stop();
}

void Game::init() {
    SetTraceLogLevel(LOG_ERROR); // opengl init comments
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);// | FLAG_WINDOW_UNDECORATED);
    InitWindow(virtualScreenSizeX, virtualScreenSizeY, "Biome Maker");
    SetTargetFPS(60);
    
    ToggleBorderlessWindowed();
    virtualScreen = LoadRenderTexture(virtualScreenSizeX, virtualScreenSizeY);
    SetTextureFilter(virtualScreen.texture, TEXTURE_FILTER_POINT);
}

void Game::stop() {
    CloseWindow();
}

Vector2 Game::getMouseVirtual() const {
    Vector2 mousePhys = GetMousePosition();

    float scaleX = (float)GetScreenWidth() / virtualScreenSizeX;
    float scaleY = (float)GetScreenHeight() / virtualScreenSizeY;

    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    int offsetX = (GetScreenWidth() - (int)(virtualScreenSizeX * scale)) / 2;
    int offsetY = (GetScreenHeight() - (int)(virtualScreenSizeY * scale)) / 2;

    Vector2 mouseVirtual = {
        (mousePhys.x - offsetX) / scale,
        (mousePhys.y - offsetY) / scale
    };

    return mouseVirtual;
}
