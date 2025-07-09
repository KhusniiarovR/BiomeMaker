#include "GameScene.h"
#include "raylib.h"
#include <Constants/GraphicsConst.h>
#include <Constants/TilemapConst.h>
#include "Items/ItemBase/ObjectToItem.h"
#include "Items/Inventory/Inventory.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "Items/ItemsAll/Tools/ItemToolBase.h"

GameScene::GameScene(Renderer& renderer, const std::string& worldName)
: Scene(renderer), 
world(worldName),
collision(world),
player ({worldSize * worldTileSize / 2.0f, worldSize * worldTileSize / 2.0f}, &collision, worldName),
enemies(player, &collision)
{
    renderer.GetCamera().offset = {virtualScreenSizeX / 2.0f, virtualScreenSizeY / 2.0f};
    player.getInventory().addItem(ItemID::AXE, 1);
    player.getInventory().addItem(ItemID::PICKAXE, 1);
}

GameScene::~GameScene() {
    player.getInventory().save();
}

void GameScene::update(float dt, Vector2 mouseVirtual) {
    updatePlayer(dt, mouseVirtual);
    updateEnemies(dt); 
    updateWorld(mouseVirtual);
    updateCamera();
    updateChangeScene();
}

void GameScene::render() const {
    world.render(renderer);
    enemies.render(renderer);
    player.render(renderer);
    player.getInventory().render(renderer);
    renderer.drawText("Enemies: " + std::to_string(enemies.getEnemiesSize()), {0.88f, 0.1f}, 20, BLACK);
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

void GameScene::updatePlayer(float dt, Vector2 mouseVirtual) {
    player.update(dt);
    player.getInventory().update(mouseVirtual);

    if (IsKeyPressed(KEY_E)) {
        ItemStack& stack = player.getInventory().getSlot(player.getInventory().selectedSlot);
        if (!stack.isEmpty()) {
            Item& item = stack.getItem();

            ItemToolBase* tool = dynamic_cast<ItemToolBase*>(&item);

            ItemUseContext context(world, player, mouseVirtual, renderer.GetCamera(), worldTileSize);
            bool used = item.onUse(context);

            if (used && tool) { 
                stack.damage(1);

                if (stack.isBroken()) {
                    stack.id = ItemID::NONE;
                    stack.count = 0;
                    stack.durability = 0;
                    return;
                }
            }

            if (used && item.shouldConsumeOnUse()) {
                if (--stack.count == 0)
                    stack.id = ItemID::NONE;
            }
        }
    }
}


void GameScene::updateEnemies(float dt) {
    enemies.update(dt);
}

void GameScene::updateWorld(Vector2 mouseVirtual) {
    world.update(player.getPosition(), renderer.GetCamera(), mouseVirtual);
}

void GameScene::updateCamera() {
    renderer.updateCameraTarget(player.getPosition());
}