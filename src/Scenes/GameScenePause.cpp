#include "GameScenePause.h"
#include "Constants/GraphicsConst.h"

PauseMenu::PauseMenu() :
resumeButton({0.3f, 0.2f} , {0.4f, 0.2f}, "RESUME",WHITE, 0.5f),
exitButton  ({0.3f, 0.6f} , {0.4f, 0.2f}, "EXIT",  WHITE, 0.5f) {}

void PauseMenu::init(std::function<void()> onResume, std::function<void()> onExit) // parameters to take functionalily from GameScene
{
    resumeButton.setOnClick(onResume);
    exitButton.setOnClick(onExit);
}

void PauseMenu::update(Vector2 mouseVirtual) 
{
    resumeButton.update(mouseVirtual);
    exitButton.update(mouseVirtual);
}

void PauseMenu::render(Renderer& renderer) const 
{
    DrawRectangle(0, 0, virtualScreenSizeX, virtualScreenSizeY, Fade(BLACK, 0.5f)); // dark rectangle to understand that game is paused

    resumeButton.render(renderer);
    exitButton.render(renderer);
}