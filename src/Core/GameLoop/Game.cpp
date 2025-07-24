#include "Game.h"
#include "raylib.h"
#include "Constants/GraphicsConst.h"
#include <cmath>

Game::Game() : renderer(assetManager, virtualScreenSizeX, virtualScreenSizeY, mouseVirtual), sceneManager(renderer)
{
    init();
}

void Game::run()
{
    while (!WindowShouldClose()) {
        /*          update logic            */
        float dt = GetFrameTime();
        mouseVirtual = getMouseVirtual();
        sceneManager.update(dt, mouseVirtual);

        /*          drawing logic            */

        BeginTextureMode(virtualScreen); // virtual screen texture
        ClearBackground(BLACK);
        sceneManager.render();
        EndTextureMode();
    

        BeginDrawing(); // transporting virtual screen to real
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

    stop(); // stopping program
}

void Game::init()
{
    SetTraceLogLevel(LOG_ERROR); // turn of opengl init comments

    //             vert sync         resize window               windows close, resize, hide buttons 
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);
    
    //any size can be given width        height         window name
    InitWindow(GetScreenWidth(), GetScreenHeight(), "Biome Maker");
    SetTargetFPS(60);     // fps
    //ToggleBorderlessWindowed(); // resize window

    virtualScreen = LoadRenderTexture(virtualScreenSizeX, virtualScreenSizeY); // virtual screen

    SetTextureFilter(virtualScreen.texture, TEXTURE_FILTER_POINT); // to avoid gpu blur

    InitAudioDevice(); // raylib sounds init
    assetManager.init(); // init default assets
}

void Game::stop()
{
    CloseAudioDevice();
    CloseWindow(); 
}

Vector2 Game::getMouseVirtual() const // mouse position that counts virtual screen sizes 
{
    Vector2 mousePhys = GetMousePosition();

    float scaleX = (float)GetScreenWidth() / virtualScreenSizeX;
    float scaleY = (float)GetScreenHeight() / virtualScreenSizeY;

    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    int offsetX = (GetScreenWidth() - (int)(virtualScreenSizeX * scale)) / 2;
    int offsetY = (GetScreenHeight() - (int)(virtualScreenSizeY * scale)) / 2;
    
    return { (mousePhys.x - offsetX) / scale, (mousePhys.y - offsetY) / scale};
}
