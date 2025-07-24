#pragma once
#include <string>
#include <vector>
#include "raylib.h"
#include "Core/Render/Renderer.h"

enum class NotificationType { Info, Warning, Error };

class NotificationManager {
public:
    struct Notification {
        std::string text;
        float timer = 0.0f;
        float duration = 3.0f;
        NotificationType type;
        Notification(const std::string& msg, NotificationType type, float durationSec = 3.0f) : text(msg), type(type), timer(durationSec), duration(durationSec) {}
    };
    
    static NotificationManager& getInstance();
    NotificationManager(const NotificationManager&) = delete;
    void operator=(const NotificationManager&) = delete;
    void show(const std::string& message, NotificationType type, float duration = 8.0f);

    void update(float dt);
    void render(Renderer& renderer) const;
    
private:
    NotificationManager() = default;
    
    std::vector<Notification> notifications;
    const int padding = 8;
    const int spacing = 6;
    const int minWidth = 100;
    const int maxWidth = 300;
    const int textSize = 20;
};
