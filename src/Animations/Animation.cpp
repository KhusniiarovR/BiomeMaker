#include "Animation.h"
#include "Utilities/Logger/Logger.h"

Animation::Animation(int frameWidth, int frameHeight) : frameWidth(frameWidth), frameHeight(frameHeight) {}

void Animation::addAnimation(const AnimationBase& anim) {
    animations[anim.name] = anim;
}

void Animation::play(const std::string& name) {
    auto it = animations.find(name);
    if (it == animations.end()) {
        mycerr << "animation " << name << "not found!";
        return;
    }

    if (currentAnimation && currentAnimation->name == name)
        return;

    currentAnimation = &animations[name];
    timer = 0.0f;
    currentFrame = 0;
}

void Animation::update(float deltaTime) {
    if (!currentAnimation) return;

    timer += deltaTime;
    if (timer >= currentAnimation->frameTime) {
        timer -= currentAnimation->frameTime;
        currentFrame++;

        if (currentFrame >= currentAnimation->frameCount) {
            currentFrame = currentAnimation->loop ? 0 : currentAnimation->frameCount - 1;
        }
    }
}

void Animation::draw(Texture2D& texture, Vector2 position) const {
    if (!currentAnimation) return;

    int frameIndex = currentAnimation->startFrame + currentFrame;
    int columns = texture.width / frameWidth;

    Rectangle src = {
        (float)((frameIndex % columns) * frameWidth),
        (float)((frameIndex / columns) * frameHeight),
        (float)frameWidth,
        (float)frameHeight
    };

    if (flip) { src.width *= -1; }

    Rectangle dest = {
        position.x,
        position.y,
        (float)frameWidth,
        (float)frameHeight
    };

    Vector2 origin = { frameWidth / 2.0f, frameHeight / 2.0f };
    DrawTexturePro(texture, src, dest, origin, 0.0f, WHITE);
}

void Animation::setFlip(bool f) {
    flip = f;
}

bool Animation::isFlipped() const {
    return flip;
}