#include "GameUi.h"
#include "raylib.h"

void GameUi::update(Vector2 mouseVirtual) {
    if (IsKeyPressed(KEY_Z)) { changeState(); }
    
    bool isFull = (state == UIState::Full);
    player->getInventory().update(mouseVirtual, isFull);
}

void GameUi::render(Renderer& renderer) const {
    player->getHP().render(renderer);
    player->getInventory().render(renderer, state == UIState::Full);
}

void GameUi::changeState() 
{
    state = (state == UIState::Compact) ? UIState::Full : UIState::Compact;
}
