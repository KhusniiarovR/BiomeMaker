#pragma once
#include "raylib.h"
#include <string>
#include "Core/Render/Renderer.h"

class BaseBar { // basic bar class
protected:
    Rectangle barBack;
    Color backColor = GRAY;
    Color fillColor = GREEN;
    float progress = 1.0f;
    std::string name;
    float fontSize = 20.0f;
    bool showPercent = false;

public:
    BaseBar(Vector2 position, Vector2 size, Color fill = GREEN, Color back = GRAY,
            std::string name = "", int fontSize = 20, bool showPercent = false);
    virtual void update(float dt) = 0;
    virtual void render(Renderer& renderer) const;

    void setProgress(float value);
    float getProgress() const;
};

class TimerBar : public BaseBar { // bar with timer
private:
    float speed;
    bool increasing;

public:
    TimerBar(Vector2 position, Vector2 size, float speed, bool isIncreasing,
             Color fill = GREEN, Color back = GRAY,
             std::string name = "", int fontSize = 20, bool showPercent = false);
    void update(float dt) override;

    void setIncreasing(bool inc); // go up or down
};

class ValueBar : public BaseBar { // bar with value
public:
    ValueBar(Vector2 position, Vector2 size, Color fill = GREEN, Color back = GRAY,
             std::string name = "", int fontSize = 20, bool showPercent = false);
    void update(float dt) override {}

    void increase(float amount);
    void decrease(float amount);
};
