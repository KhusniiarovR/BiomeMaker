#ifndef WORLDSELECTIONSCENE_H
#define WORLDSELECTIONSCENE_H

#include "Scene.h"
#include "World/WorldMenu/WorldSelector.h"
#include "World/WorldMenu/WorldCreator.h"
#include "Utilities/Graphics/Button.h"
#include "Utilities/Graphics/TextBox.h"
#include "Utilities/Graphics/Bar.h"

class WorldSelectionScene : public Scene {
private:
    WorldSelector worldSelector;
    WorldCreator worldCreator;

    Button playButton;
    Button createNewButton;
    Button deleteButton;

    Button createButton;
    Button createRandButton;
    Button backButton;
    TextBox enterName;

    TimerBar timer;

    bool firstPage = true;
    bool generationStage = false;

    std::atomic<bool> generationFinished = false;

public:
    explicit WorldSelectionScene(Renderer& renderer);

    void update(float dt, Vector2 mouseVirtual) override;
    void render() const override;

    void updateChangeScene() override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
    [[nodiscard]] std::string getWorldName() const override;

private:    
    void eraseWorldName();
};

#endif //WORLDSELECTIONSCENE_H
