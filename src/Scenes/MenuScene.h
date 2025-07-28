#pragma once
#include "Scene.h"
#include "Utilities/Graphics/Button.h"
#include "Settings/Settings.h"

class MenuScene : public Scene { 
private:
    Button playButton;
    Button settingsButton;
    Button quitButton;
    Settings settings;

public:
    MenuScene(Renderer& renderer);

    void update(float dt, Vector2 mouseVirtual) override;
    void render() const override;

    void updateChangeScene() override;
    [[nodiscard]] bool shouldTransition() const override;
    [[nodiscard]] SceneType getNextScene() const override;
};