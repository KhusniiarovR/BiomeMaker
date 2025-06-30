#include "Bar.h"
#include <algorithm>
#include "Constants/GraphicsConst.h"

BaseBar::BaseBar(Vector2 position, Vector2 size, Color fill, Color back, 
                 std::string name, int fontSize, bool showPercent)
        : barBack({position.x * virtualScreenSizeX, position.y * virtualScreenSizeY, size.x * virtualScreenSizeX, size.y * virtualScreenSizeY}), 
          fillColor(fill), backColor(back), name(name), fontSize(fontSize), showPercent(showPercent) {}

void BaseBar::render(Renderer& renderer) const {
    DrawRectangleRec(barBack, backColor);
    Rectangle barFill = barBack;
    barFill.width *= progress;
    DrawRectangleRec(barFill, fillColor);

    std::string displayText = name;

    if (showPercent) {
        int percent = static_cast<int>(progress * 100.0f + 0.5f);
        displayText += (name.empty() ? "" : " ") + std::to_string(percent) + "%";
    }

    if (!displayText.empty()) {
        renderer.drawText(displayText, {barBack.x + barBack.width/2.0f, barBack.y + barBack.height/2.0f}, fontSize, BLACK, true, false);
    }
}

void BaseBar::setProgress(float value) {
    progress = std::clamp(value, 0.0f, 1.0f);
}

float BaseBar::getProgress() const {
    return progress;
}

TimerBar::TimerBar(Vector2 position, Vector2 size, float speed, bool isIncreasing,
                   Color fill, Color back, std::string name, int fontSize, bool showPercent)
    : BaseBar(position, size, fill, back, name, fontSize, showPercent),
      speed(speed), increasing(isIncreasing) {}

void TimerBar::update(float dt) {
    float delta = speed * dt;
    if (increasing)
        progress = std::min(1.0f, progress + delta);
    else
        progress = std::max(0.0f, progress - delta);
}

void TimerBar::setIncreasing(bool isIncreasing) {
    increasing = isIncreasing;
}


ValueBar::ValueBar(Vector2 position, Vector2 size,
                   Color fill, Color back, std::string name, int fontSize, bool showPercent)
    : BaseBar(position, size, fill, back, name, fontSize, showPercent) {}

void ValueBar::increase(float amount) {
    progress = std::min(1.0f, progress + amount);
}

void ValueBar::decrease(float amount) {
    progress = std::max(0.0f, progress - amount);
}
