#pragma once
#include "Core/Render/Renderer.h"
#include "Entities/Player/Player.h"

enum class UIState {
    Compact,
    Full
};

class GameUi { // in game states (inventory)
private:
    UIState state = UIState::Compact;
    Player* player = nullptr;

public:
    void update(Vector2 mouseVirtual);
    void render(Renderer& renderer) const;
    void setPlayer(Player* p) { player = p; }
    
private:
    void changeState();
};