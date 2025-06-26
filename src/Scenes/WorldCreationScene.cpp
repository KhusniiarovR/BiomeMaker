#include "WorldCreationScene.h"
#include <thread>
WorldCreationScene::WorldCreationScene(Renderer &renderer): Scene(renderer),
    createButton({0.1f, 0.55f}, {0.2f, 0.1f}, "CREATE", BLACK, 0.5f),
    backButton({0.7f, 0.55f}, {0.2f, 0.1f}, "BACK", BLACK, 0.5f),
    createRandButton({0.4f, 0.55f}, {0.2f, 0.1f}, "RANDOM", BLACK, 0.5f),
    enterName(0.42f,0.14f,0.21f,0.1f,WHITE, RED)
    {
        createButton.setOnClick([this]() 
        {
            std::thread([this]() 
            {
                if(!enterName.returnText().empty())
                {
                    worldCreator.generate(enterName.returnText());
                    worldSelector.loadFolders();
                    changeScene = true;
                    nextScene = SceneType::WorldSelection;
                }
            }).join();
        });
        backButton.setOnClick([this]()
        {
            std::thread([this]()
            {
                changeScene = true;
                nextScene = SceneType::WorldSelection;
            }).join();
        });
        createRandButton.setOnClick([this]()
        {
            std::thread([this]() 
            {
                worldCreator.generate();
                worldSelector.loadFolders();
                changeScene = true;
                nextScene = SceneType::WorldSelection;
            }).join();
        });
    }
void WorldCreationScene::update(float dt) 
{
    createButton.update();
    backButton.update();
    createRandButton.update();
    enterName.update(dt);
}
void WorldCreationScene::render() const 
{
    createRandButton.render(renderer);
    backButton.render(renderer);
    createButton.render(renderer);
    enterName.draw(renderer);
}

bool WorldCreationScene::shouldTransition() const 
{
    return changeScene;
}

SceneType WorldCreationScene::getNextScene() const 
{
    return nextScene;
}
std::string WorldCreationScene::getWorldName() const 
{
    return worldSelector.getSelectedFolder();
}