#include "WorldCreationScene.h"
#include <thread>
WorldCreationScene::WorldCreationScene(Renderer &renderer)
    : Scene(renderer),
    createButton({0.6f, 0.55f}, {0.2f, 0.1f}, "CREATE", BLACK, 0.5f),
    enterName(800,150,400,200,WHITE, RED)
    {
        createButton.setOnClick([this]() {
        std::thread([this]() {
        worldCreator.generate();
        worldSelector.loadFolders();
        changeScene = true;
        nextScene = SceneType::WorldSelection;
        }).detach();
    });
    }
void WorldCreationScene::update(float dt) {
    createButton.update();
    enterName.update();
}
void WorldCreationScene::render() const {
    createButton.render(renderer);
    enterName.draw(renderer);
}

bool WorldCreationScene::shouldTransition() const {
    return changeScene;
}

SceneType WorldCreationScene::getNextScene() const {
    return nextScene;
}
std::string WorldCreationScene::getWorldName() const {
    return worldSelector.getSelectedFolder();
}