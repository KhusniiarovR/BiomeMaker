#ifndef GAMEUI_H
#define GAMEUI_H

#include "Core/Render/Renderer.h"
#include "Entities/Player/Player.h"

enum class UIState {
    Compact,
    Full
};

class GameUi {
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

#endif // GAMEUI_H