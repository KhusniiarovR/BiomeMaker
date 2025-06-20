#pragma once 
#include <string>
#include "Scene.h"
#include <Core/World/WorldSelector.h>
#include "Core/World/WorldCreator.h"
#include "Utilities/Graphics/Button.h"
#include "Utilities/Graphics/TextBox.h"
class WorldCreationScene : public Scene
{
    private:
        WorldCreator worldCreator;
        WorldSelector worldSelector;
        Button backButton;
        Button createRandButton;
        Button createButton;
        TextBox enterName;
        std::string worldName;

    public:
        explicit WorldCreationScene(Renderer& renderer);
        void update(float dt) override;
        void render() const override;
        [[nodiscard]] bool shouldTransition() const override;
        [[nodiscard]] SceneType getNextScene() const override;
        [[nodiscard]] std::string getWorldName() const override;
};