#include "WorldSelectionScene.h"
#include <thread>

WorldSelectionScene::WorldSelectionScene(Renderer &renderer)
    : Scene(renderer),
    playButton      ({0.6f, 0.3f},  {0.2f, 0.1f}, "PLAY",   BLACK, 0.5f),
    createNewButton ({0.6f, 0.55f}, {0.2f, 0.1f}, "CREATE", BLACK, 0.5f),
    deleteButton    ({0.6f, 0.8f},  {0.2f, 0.1f}, "DELETE", BLACK, 0.5f),
    createButton    ({0.1f, 0.55f}, {0.2f, 0.1f}, "CREATE", WHITE, 0.5f),
    createRandButton({0.4f, 0.55f}, {0.2f, 0.1f}, "RANDOM", WHITE, 0.5f),
    backButton      ({0.7f, 0.55f}, {0.2f, 0.1f}, "BACK",   WHITE, 0.5f),
    enterName       (0.3f , 0.2f ,   0.4f ,0.05f, BLACK, RED, SKYBLUE, WHITE, 20)
{
    playButton.setOnClick([this]() {
        if (!worldSelector.getSelectedFolder().empty()) {
            changeScene = true;
            nextScene = SceneType::Game;
        }
    });
    createNewButton.setOnClick([this]() {
        firstPage = false;
        eraseWorldName();
    });
    deleteButton.setOnClick([this]() {
        worldSelector.deleteCurrent();
    });

    createButton.setOnClick([this]() 
    {
        if(enterName.returnText().empty()) return;

        generationStage = true;
        generationFinished = false;
        fakeTimer = 0.0f;
        std::thread([this]() 
        {
            {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                worldCreator.generate(enterName.returnText());
                worldSelector.loadFolders();
                firstPage = true;
                generationStage = false;
                generationFinished = true;
            }
        }).detach();
    });
    createRandButton.setOnClick([this]()
    {
        generationStage = true;
        generationFinished = false;
        fakeTimer = 0.0f;
        std::thread([this]() 
        {
            {
                std::this_thread::sleep_for(std::chrono::seconds(5));
                worldCreator.generate();
                worldSelector.loadFolders();
                firstPage = true;
                generationStage = false;
                generationFinished = true;
            }
        }).detach();
    });
    backButton.setOnClick([this]() { firstPage = true; });

    playButton.setTexture(renderer.getTexture("button1"));
    createNewButton.setTexture(renderer.getTexture("button1"));
    deleteButton.setTexture(renderer.getTexture("button1"));
}

void WorldSelectionScene::update(float dt, Vector2 mouseVirtual) {
    if (firstPage) {
        worldSelector.update(mouseVirtual);
        playButton.update(mouseVirtual);
        createNewButton.update(mouseVirtual);
        deleteButton.update(mouseVirtual);
    }
    else {
        if (!generationStage) {
            createButton.update(mouseVirtual);
            createRandButton.update(mouseVirtual);
            backButton.update(mouseVirtual);
            enterName.update(dt, mouseVirtual);
        }
        else {
            fakeTimer += dt;
            if (fakeTimer >= totalFakeTime && generationFinished) {
                worldSelector.loadFolders();
                firstPage = true;
                generationStage = false;
            }
        }   
    }

    updateChangeScene();
}

void WorldSelectionScene::render() const {
    renderer.drawBackground();

    if (firstPage) {
        worldSelector.render(renderer);
        playButton.render(renderer);
        createNewButton.render(renderer);
        deleteButton.render(renderer);
    }
    else {
        if (!generationStage) {
            renderer.drawTextGradient("World Name: ", {0.5, 0.15}, 20, 2.0f, BLACK, YELLOW, true, true, 0.0f, 0.0f);
            createRandButton.render(renderer);
            backButton.render(renderer);
            createButton.render(renderer);
            enterName.draw(renderer);
        }
        else {
            renderer.drawTextGradient("Generating World...", {0.5, 0.15}, 20, 2.0f, RED, PURPLE, true, true, 0.0f, 0.0f);

            float progress = std::min(fakeTimer / totalFakeTime, 0.9f);
            Rectangle barBack = { 0.2f * virtualScreenSizeX, 0.2f * virtualScreenSizeY, 0.6f * virtualScreenSizeX, 0.3f * virtualScreenSizeX,};
            Rectangle barFill = barBack;
            barFill.width *= progress;

            DrawRectangleRec(barBack, GRAY);
            DrawRectangleRec(barFill, GREEN);
        }
    }
}

void WorldSelectionScene::updateChangeScene() {
    if (IsKeyPressed(KEY_ENTER) && !generationStage) {
        if (!worldSelector.getSelectedFolder().empty()) {
            changeScene = true;
            nextScene = SceneType::Game;
        }
    }

    if (IsKeyPressed(KEY_SPACE)  && !generationStage) {
        changeScene = true;
        nextScene = SceneType::MainMenu;
    }
}

bool WorldSelectionScene::shouldTransition() const {
    return changeScene;
}

SceneType WorldSelectionScene::getNextScene() const {
    return nextScene;
}

std::string WorldSelectionScene::getWorldName() const {
    return worldSelector.getSelectedFolder();
}

void WorldSelectionScene::eraseWorldName() {
    enterName.eraseText();
}