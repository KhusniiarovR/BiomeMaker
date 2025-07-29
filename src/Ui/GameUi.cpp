#include "GameUi.h"
#include "raylib.h"
#include "Settings/InputManager.h"

void GameUi::update(Vector2 mouseVirtual) 
{
    if (InputManager::GetInstance().IsActionPressed(Action::INVENTORY)) { changeState(); }
    
    bool isFull = (state == UIState::Full);
    player->getInventory().update(mouseVirtual, isFull);
}

void GameUi::render(Renderer& renderer) const 
{
    player->getHP().render(renderer);
    player->getInventory().render(renderer, state == UIState::Full);
}

void GameUi::changeState() 
{
    state = (state == UIState::Compact) ? UIState::Full : UIState::Compact;
}
