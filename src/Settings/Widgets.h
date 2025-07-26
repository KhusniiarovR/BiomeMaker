#pragma once
#include "raylib.h"
#include <string>
#include "InputManager.h"

class SettingWidget {
public:
    virtual void Update(Vector2 mouseVirtual, float y) = 0;
    virtual void Render(float y) const = 0;
    virtual int GetHeight() const = 0;
    virtual ~SettingWidget() = default;
};

class SectionHeader : public SettingWidget {
public:
    explicit SectionHeader(const std::string& title);
    void Update(Vector2 mouseVirtual, float scrollOffset) override {}
    void Render(float scrollOffset) const override;
    int GetHeight() const override;
private:
    std::string title;
};

class IntField : public SettingWidget {
public:
    IntField(const std::string& label, int* value, int minVal, int maxVal);
    void Update(Vector2 mouseVirtual, float scrollOffset) override;
    void Render(float scrollOffset) const override;
    int GetHeight() const override;
private:
    std::string label;
    int* value;
    int min, max;
    bool editing = false;
    std::string buffer;
};

class KeyBindField : public SettingWidget {
public:
    explicit KeyBindField(Action action);
    void Update(Vector2 mouseVirtual, float scrollOffset) override;
    void Render(float scrollOffset) const override;
    int GetHeight() const override;
private:
    Action action;
    bool waitingForKey = false;
};