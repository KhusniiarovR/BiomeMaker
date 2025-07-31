#pragma once
#include "Scene.h"
#include "World/WorldMenu/WorldSelector/WorldSelector.h"
#include "World/WorldMenu/WorldCreator/WorldCreator.h"
#include "Utilities/Graphics/Button.h"
#include "Utilities/Graphics/TextBox.h"
#include "Utilities/Graphics/Bar.h"
#include <atomic>
#include "Settings/Settings.h"

class WorldSelectionScene : public Scene {
private:
    bool firstPage = true; // current page

    /* first page */
    WorldSelector worldSelector; // left side with worlds list
    
    Button playButton; // go to game scene
    Button createNewButton; // change to second page
    Button deleteButton; // delete world
    
    /* second page */
    TextBox worldName;
    Button createButton;    // create world with worldName
    Button createRandButton;// create world with random name
    Button backButton;      // change to fisrt page
    
    // world creation
    WorldCreator worldCreator; // creates worlds
    TimerBar timer;
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
    void createWorld(bool hasName);   
    void play();
};
