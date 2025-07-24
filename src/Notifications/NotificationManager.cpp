#include "NotificationManager.h"
#include "Constants/GraphicsConst.h"
#include <algorithm>
#include "Utilities/Input/LineSeparator.h"
#include "Core/Render/AssetManager.h"

NotificationManager& NotificationManager::getInstance() 
{
    static NotificationManager instance;
    return instance;
}

void NotificationManager::show(const std::string& message, NotificationType type, float duration) 
{
    notifications.emplace_back(message, type, duration);
    switch (type) 
    {
        case NotificationType::Info:    PlaySound(AssetManager::instance().getSound("notification")); break;
        case NotificationType::Warning: PlaySound(AssetManager::instance().getSound("notification")); break;
        case NotificationType::Error:   PlaySound(AssetManager::instance().getSound("notification")); break;
    }
}

void NotificationManager::update(float dt) 
{
    for (auto& n : notifications) { n.timer -= dt; }

    notifications.erase(std::remove_if(notifications.begin(), notifications.end(), [](const Notification& n) { return n.timer <= 0.0f; }), notifications.end());
}

void NotificationManager::render(Renderer& renderer) const
{
    int y = 10 + padding;
    for (size_t i = 0; i < notifications.size(); ++i) 
    {
        const Notification& n = notifications[i];
        float alpha = std::clamp(n.timer / n.duration, 0.0f, 1.0f);

        Color textColor = Fade(DARKGRAY, alpha);
        Color bgColor;
        //std::string prefix; TODO prefix icons
        switch (n.type) 
        {
            case NotificationType::Info:
                bgColor = Fade(BLUE, alpha);
                //prefix = "ℹ️ ";
                break;
            case NotificationType::Warning: 
                bgColor = Fade(ORANGE,alpha);
                //prefix = "⚠️ ";
                break;
            case NotificationType::Error:
                bgColor = Fade(RED, alpha);
                //prefix = "❌ ";
                break;
        }

        Font& font = renderer.getFont("silkscreen", textSize);
        auto lines = separateText(n.text, font, textSize, maxWidth - padding * 2);
        int lineHeight = static_cast<int>(textSize * 1.1f);

        float maxLineWidth = 0.0f;
        for (const auto& line : lines) 
        {
            float w = MeasureTextEx(font, line.c_str(), textSize, 4).x;
            if (w > maxLineWidth) maxLineWidth = w;
        }

        int boxWidth = std::clamp((int)(maxLineWidth + padding * 2), minWidth, maxWidth);
        int boxHeight = (int)(lines.size() * lineHeight + padding * 2);

        int x = 10;

        DrawRectangleRounded({ (float)x, (float)y, (float)boxWidth, (float)boxHeight }, 0.2f, 6, bgColor);

        for (size_t li = 0; li < lines.size(); ++li) 
        {
            renderer.drawText(lines[li], { (float)x + padding, (float)y + padding + li * lineHeight}, 
            textSize, textColor, false, false);
        }
        y += boxHeight + spacing;
    }
}
