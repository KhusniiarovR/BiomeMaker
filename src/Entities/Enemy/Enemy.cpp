#include "Enemy.h"
#include "Constants/TilemapConst.h"
#include "Utilities/Logger/Logger.h"
#include <cmath>

Enemy::Enemy(Vector2 initPos, Player &player, const CollisionBase* collision) 
: Entity(initPos), 
player(player), 
collision(collision),
animation(entityTileSize, entityTileSize) 
{
    animation.addAnimation({"idle", 0, 4, 0.4f, true});
    animation.addAnimation({"walkDown", 4, 4, 0.1f, true}); // todo not player textures
}

void Enemy::update(float dt) 
{
    // movement
    Vector2 toPlayer = { player.position.x - position.x, player.position.y - position.y};
    float dist = sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);
    
    if (dist < 1.0f) { animation.play("idle"); animation.update(dt); return; } // inside player

    float moveSpeed = speed * dt * speedMultiplier;
    float dx = toPlayer.x / dist * moveSpeed; 
    float dy = toPlayer.y / dist * moveSpeed;

    bool moved = tryMove(dx, dy);
    if (moved)
    {   
        float absX = fabsf(dx); 
        float absY = fabsf(dy);

        if (absX > absY) // horizontally
        { 
            animation.play("walkDown");
            animation.setFlip(dx < 0); // left or right
        } 
        else // vertically
        { 
            if (dy < 0) { animation.play("walkDown"); } // up
            else { animation.play("walkDown"); } // down
        }
    }
    else { animation.play("idle"); }

    animation.update(dt);
}

void Enemy::render(Renderer& renderer) const 
{
    animation.render(renderer.getTexture("playerTilemap"), position);
}

Vector2 Enemy::getPosition() const 
{
    return position;
}

Rectangle Enemy::getBoundingBox() const 
{
    float width = 0.45f * entityTileSize;
    float height = 0.9f * entityTileSize;
    return { position.x - width / 2.0f, position.y - height / 2.0f, width, height };
}

bool Enemy::tryMove(float dx, float dy) 
{
    bool moved = false;
    Rectangle oldBox = getBoundingBox();
    Rectangle newBox = oldBox;
    newBox.x += dx;
    newBox.y += dy;

    if (!collision || !collision->checkCollision(newBox)) 
    {
        position.x += dx;
        position.y += dy;
        moved = true;
    }
    else 
    {
        newBox = oldBox;
        newBox.x += dx;
        if (!collision->checkCollision(newBox)) { position.x += dx; moved = true; }

        newBox = oldBox;
        newBox.y += dy;
        if (!collision->checkCollision(newBox)) { position.y += dy; moved = true; }
    }
    return moved;
}