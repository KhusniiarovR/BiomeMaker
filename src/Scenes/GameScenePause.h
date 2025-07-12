#ifndef GAMESCENEPAUSE_H
#define GAMESCENEPAUSE_H

#include "Utilities/Graphics/Button.h"
#include "Core/Render/Renderer.h"

class PauseMenu {
private:    
    Button resumeButton;
    Button exitButton;

public:
    PauseMenu();
    void init(std::function<void()> onResume, std::function<void()> onExit);

    void update(Vector2 mouseVirtual);
    void render(Renderer& renderer) const;
};

#endif // GAMESCENEPAUSE_H