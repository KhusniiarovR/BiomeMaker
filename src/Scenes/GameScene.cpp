#include "GameScene.h"
#include "raylib.h"
#include <Constants/GraphicsConst.h>
#include <Constants/TilemapConst.h>
#include "Items/ItemBase/ObjectToItem.h"
#include "Items/Inventory/Inventory.h"
#include "Items/ItemBase/ItemUseContext.h"
#include "Items/ItemsAll/Tools/ItemToolBase.h"
#include "Utilities/Logger/Logger.h"
#include "Settings/InputManager.h"

GameScene::GameScene(Renderer& renderer, const std::string& worldName)
: Scene(renderer), 
world(worldName),
player ({worldSize * worldTileSize / 2.0f, worldSize * worldTileSize / 2.0f}, &collision, worldName),
enemies(player, &collision)
{
    // set collisions
    provider.setWorld(&world);
    provider.setPlayer(&player);
    provider.setEnemies(&enemies);
    collision.setProvider(&provider);

    // camera centralized on a player
    renderer.GetCamera().offset = {virtualScreenSizeX / 2.0f, virtualScreenSizeY / 2.0f};

    // player and inventory
    player.getInventory().addItem(ItemID::AXE, 1);
    player.getInventory().addItem(ItemID::PICKAXE, 1);
    ui.setPlayer(&player);

    // pause menu
    pauseMenu.init([this]() { paused = false; }, [this]() {changeScene = true; nextScene = SceneType::Menu;} );
}

GameScene::~GameScene() {}

void GameScene::update(float dt, Vector2 mouseVirtual) 
{
    // pause logic
    updatePause(mouseVirtual);
    if (paused) { return; }

    updatePlayer(dt, mouseVirtual); // player
    updateEnemies(dt);              // enemy
    updateWorld(mouseVirtual);      // world
    updateCamera();                 // camera
    updateChangeScene();            // scene
}

void GameScene::render() const 
{
    world.render(renderer);   // world
    enemies.render(renderer); // enemy
    player.render(renderer);  // player

    // ending mode 2d for rendering ui
    EndMode2D();
    is2DModeDone = true;
    ui.render(renderer);

    if (paused) { pauseMenu.render(renderer); } // pause
}

void GameScene::updateChangeScene() {}

bool GameScene::shouldTransition() const 
{
    return changeScene;
}

SceneType GameScene::getNextScene() const 
{
    return nextScene;
}

void GameScene::updatePlayer(float dt, Vector2 mouseVirtual) 
{
    player.update(dt);
    ui.update(mouseVirtual);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && player.getInventory().getHoveredSlot() == -1) // to ignore click if mouse is in the inventory
    { 
        ItemStack& stack = player.getInventory().getSelectedSlot();
        Item& item = stack.getItem();   
        ItemToolBase* tool = dynamic_cast<ItemToolBase*>(&item);
        ItemUseContext context(world, player, mouseVirtual, renderer.GetCamera());
        bool used = item.onUse(context);

        if (used && tool) 
        { 
            stack.damage(1);
            if (stack.isBroken()) 
            {
                stack.id = ItemID::NONE;
                stack.count = 0;
                stack.durability = 0;
                return;
            }
        }

        if (used && item.shouldConsumeOnUse()) { if (--stack.count == 0) stack.id = ItemID::NONE; }
    }
}

void GameScene::updateEnemies(float dt) 
{
    enemies.update(dt);
}

void GameScene::updateWorld(Vector2 mouseVirtual) 
{
    world.update(player.getPosition(), renderer.GetCamera(), mouseVirtual);
}

void GameScene::updateCamera() 
{
    renderer.updateCameraTarget(player.getPosition());
}

void GameScene::updatePause(Vector2 mouseVirtual) 
{
    if (InputManager::GetInstance().IsActionPressed(Action::ESCAPE)) { paused = !paused; }

    if (paused) { pauseMenu.update(mouseVirtual); }
}