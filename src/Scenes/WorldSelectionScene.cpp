#include "WorldSelectionScene.h"
#include <thread>
#include "Notifications/NotificationManager.h"
#include "Settings/InputManager.h"

WorldSelectionScene::WorldSelectionScene(Renderer &renderer)
: Scene(renderer),
playButton      ({0.6f, 0.3f} , {0.2f, 0.1f}, "PLAY",   BLACK, 0.5f),
createNewButton ({0.6f, 0.55f}, {0.2f, 0.1f}, "CREATE", BLACK, 0.5f),
deleteButton    ({0.6f, 0.8f} , {0.2f, 0.1f}, "DELETE", BLACK, 0.5f),
createButton    ({0.1f, 0.55f}, {0.2f, 0.1f}, "CREATE", WHITE, 0.5f),
createRandButton({0.4f, 0.55f}, {0.2f, 0.1f}, "RANDOM", WHITE, 0.5f),
backButton      ({0.7f, 0.55f}, {0.2f, 0.1f}, "BACK",   WHITE, 0.5f),
worldName       ( 0.3f , 0.2f ,  0.4f ,0.05f, BLACK, RED, SKYBLUE, WHITE, 20),
timer           ({0.2f, 0.2f} , {0.6f, 0.3f}, 1.0f/3.0f, true, GREEN, GRAY, "", 20, true)
{
    // first page buttons
    playButton.setOnClick([this]() { play(); });
    createNewButton.setOnClick([this]() { firstPage = false; worldName.eraseText(); });
    deleteButton.setOnClick([this]() { worldSelector.deleteSelected(); });

    // second page buttons
    createButton.setOnClick([this]() { createWorld(true); });
    createRandButton.setOnClick([this]() { createWorld(false); });
    backButton.setOnClick([this]() { firstPage = true; });

    // button textures
    playButton.setTexture(renderer.getTexture("button1"));
    createNewButton.setTexture(renderer.getTexture("button1"));
    deleteButton.setTexture(renderer.getTexture("button1"));
}

void WorldSelectionScene::update(float dt, Vector2 mouseVirtual) 
{
    if (firstPage) 
    {
        worldSelector.update(mouseVirtual);
        playButton.update(mouseVirtual);
        createNewButton.update(mouseVirtual);
        deleteButton.update(mouseVirtual);
    }
    else 
    {
        if (!generationStage) 
        {
            createButton.update(mouseVirtual);
            createRandButton.update(mouseVirtual);
            backButton.update(mouseVirtual);
            worldName.update(dt, mouseVirtual);
        }
        else 
        {
            timer.update(dt);
            if (timer.getProgress() == 1.0f && generationFinished) 
            {
                worldSelector.loadFolders();
                firstPage = true;
                generationStage = false;
            }
        }   
    }
    updateChangeScene();
}

void WorldSelectionScene::render() const 
{
    renderer.drawBackground();

    if (firstPage) 
    {
        worldSelector.render(renderer);
        playButton.render(renderer);
        createNewButton.render(renderer);
        deleteButton.render(renderer);
    }
    else 
    {
        if (!generationStage) 
        {
            renderer.drawTextGradient("World Name: ", {0.5, 0.15}, 20, 2.0f, BLACK, YELLOW, true, true, 0.0f, 0.0f);
            createRandButton.render(renderer);
            backButton.render(renderer);
            createButton.render(renderer);
            worldName.draw(renderer);
        }
        else 
        {
            renderer.drawTextGradient("Generating World...", {0.5, 0.15}, 20, 2.0f, RED, PURPLE, true, true, 0.0f, 0.0f);
            timer.render(renderer);
        }
    }
}

void WorldSelectionScene::updateChangeScene() 
{
    if (InputManager::GetInstance().IsActionPressed(Action::ENTER) && !generationStage) { play(); }

    if (InputManager::GetInstance().IsActionPressed(Action::ESCAPE) && !generationStage) 
    {
        changeScene = true;
        nextScene = SceneType::Menu;
    } // todo back button
}

bool WorldSelectionScene::shouldTransition() const 
{
    return changeScene;
}

SceneType WorldSelectionScene::getNextScene() const 
{
    return nextScene;
}

std::string WorldSelectionScene::getWorldName() const 
{
    return worldSelector.getSelectedFolder();
}

void WorldSelectionScene::createWorld(bool hasName) 
{
    if (hasName) 
    {
        if(worldName.getText().empty()) 
        {
            NotificationManager::getInstance().show("World name must be not empty", NotificationType::Warning); 
            return;
        }
    }

    generationStage = true;
    generationFinished = false;
    timer.setProgress(0.0f);
    std::thread([this, hasName]() 
    {
        {
            (hasName) ? worldCreator.generate(worldName.getText()) : worldCreator.generate();
            worldSelector.loadFolders();
            firstPage = true;
            generationStage = false;
            generationFinished = true;
        }
    }).detach();
}

void WorldSelectionScene::play() 
{
    if (!worldSelector.getSelectedFolder().empty()) 
    {
        changeScene = true;
        nextScene = SceneType::Game;
    }
    else { NotificationManager::getInstance().show("Choose world to play!", NotificationType::Info, 6.0f); }
}