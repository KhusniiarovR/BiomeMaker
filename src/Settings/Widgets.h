#pragma once
#include "raylib.h"
#include <string>
#include "InputManager.h"
#include "Core/Render/Renderer.h"

class SettingWidget {
public:
    virtual void update(Vector2 mouseVirtual, float y) = 0;
    virtual void render(Renderer& renderer, float y) const = 0;
    virtual int getHeight() const = 0;
    virtual ~SettingWidget() = default;
};

class SectionHeader : public SettingWidget { // big section name
private:
    std::string title;

public:
    explicit SectionHeader(const std::string& title);
    void update(Vector2 mouseVirtual, float scrollOffset) override {}
    void render(Renderer& renderer, float scrollOffset) const override;
    int getHeight() const override;
};

class IntField : public SettingWidget { // field with number and "+" "-"
private:
    std::string label;
    int* value;
    int min, max;
    bool editing = false;
    std::string buffer;

public:
    IntField(const std::string& label, int* value, int minVal, int maxVal);
    void update(Vector2 mouseVirtual, float scrollOffset) override;
    void render(Renderer& renderer, float scrollOffset) const override;
    int getHeight() const override;
};

class KeyBindField : public SettingWidget { // field with key
private:
    Action action;
    bool waitingForKey = false;
    float width = 130.0f;

public:
    explicit KeyBindField(Action action);
    void update(Vector2 mouseVirtual, float scrollOffset) override;
    void render(Renderer& renderer, float scrollOffset) const override;
    int getHeight() const override;
};