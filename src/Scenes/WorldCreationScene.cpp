#include "WorldCreationScene.h"
#include <thread>
WorldCreationScene::WorldCreationScene(Renderer &renderer): Scene(renderer),
    createButton({0.1f, 0.55f}, {0.2f, 0.1f}, "CREATE", WHITE, 0.5f),
    backButton({0.7f, 0.55f}, {0.2f, 0.1f}, "BACK", WHITE, 0.5f),
    createRandButton({0.4f, 0.55f}, {0.2f, 0.1f}, "RANDOM", WHITE, 0.5f),
    enterName(0.3f,0.2f,0.4f,0.05f, BLACK, RED, WHITE, 20)
    {
        createButton.setOnClick([this]() 
        {
            std::thread([this]() 
            {
                if(!enterName.returnText().empty())
                {
                    worldCreator.generate(enterName.returnText());
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
                changeScene = true;
                nextScene = SceneType::WorldSelection;
            }).join();
        });
    }
void WorldCreationScene::update(float dt, Vector2 mouseVirtual) 
{
    createButton.update(mouseVirtual);
    backButton.update(mouseVirtual);
    createRandButton.update(mouseVirtual);
    enterName.update(dt, mouseVirtual);
}
void WorldCreationScene::render() const 
{
    renderer.drawBackground();
    renderer.drawTextGradient("World Name: ", {0.5, 0.15}, 20, 2.0f, BLACK, YELLOW, true, true, 0.0f, 0.0f);
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