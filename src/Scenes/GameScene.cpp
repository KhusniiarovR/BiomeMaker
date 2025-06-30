#include "GameScene.h"
#include "raylib.h"
#include <Constants/GraphicsConst.h>
#include <Constants/TilemapConst.h>
#include "Items/ObjectToItem.h"

GameScene::GameScene(Renderer& renderer, const std::string& worldName) :
         Scene(renderer),
         player ({worldSize * worldTileSize / 2.0f, worldSize * worldTileSize / 2.0f}),
         enemy ({worldSize * worldTileSize / 2.0f, worldSize * worldTileSize / 2.0f}, player),
         world(worldName)
{
    renderer.GetCamera().offset = {virtualScreenSizeX / 2.0f, virtualScreenSizeY / 2.0f};
}

void GameScene::update(float dt, Vector2 mouseVirtual) {
    updatePlayer(dt);
    updateEnemies(dt);
    updateObjects(mouseVirtual);    
    updateWorld(mouseVirtual);
    updateCamera();
    updateChangeScene();
}

void GameScene::render() const {
    world.render(renderer);
    enemy.render(renderer);
    player.render(renderer);
}

void GameScene::updateChangeScene() {
    if (IsKeyPressed(KEY_SPACE)) {
        changeScene = true;
        nextScene = SceneType::MainMenu;
    }
}

bool GameScene::shouldTransition() const {
    return changeScene;
    // TODO pause
}

SceneType GameScene::getNextScene() const {
    return nextScene;
}

void GameScene::updatePlayer(float dt) {
    player.update(dt);
}

void GameScene::updateEnemies(float dt) {
    enemy.update(dt);
}

void GameScene::updateObjects(Vector2 mouseVirtual) {
    if (IsKeyPressed(KEY_E)) {
        Vector2 mouseWorld = GetScreenToWorld2D(mouseVirtual, renderer.GetCamera());

        int tileX = static_cast<int>(mouseWorld.x / worldTileSize);
        int tileY = static_cast<int>(mouseWorld.y / worldTileSize);

        Vector2 playerPos = player.getPosition();
        int playerTileX = static_cast<int>(playerPos.x / worldTileSize);
        int playerTileY = static_cast<int>(playerPos.y / worldTileSize);

        int dx = tileX - playerTileX;
        int dy = tileY - playerTileY;

        if (dx * dx + dy * dy <= handDistance * handDistance) {
            auto removed = world.removeObjectAt(tileX, tileY);
            if (removed) {
                std::vector<ItemID> drops = generateLootForObject(*removed);
                for (ItemID id : drops) {
                    player.giveItem(id, 1);
                }
            }
        }
    }
}

void GameScene::updateWorld(Vector2 mouseVirtual) {
    world.update(player.getPosition(), renderer.GetCamera(), mouseVirtual);
}

void GameScene::updateCamera() {
    renderer.updateCamera(player.getPosition());
}