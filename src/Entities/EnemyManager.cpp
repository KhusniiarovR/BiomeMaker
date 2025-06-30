#include "EnemyManager.h"
#include <iostream>
void moveToPlayer(Player &player, Vector2 &position, float speed, std::string& currentDir){
    Vector2 dir = { player.position.x - position.x, player.position.y - position.y };
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length > 0)
    {
        dir.x /= length;
        dir.y /= length;
        position.x += dir.x * speed;
        position.y += dir.y * speed;
    }
    float absX = fabsf(dir.x);
    float absY = fabsf(dir.y);

    if (absX > absY) {
        if (dir.x > 0)
            currentDir = "Right";
        else
            currentDir = "Left";
    } 
    else {
        if (dir.y > 0)
            currentDir = "Down";
        else
            currentDir = "Up";
}
}
