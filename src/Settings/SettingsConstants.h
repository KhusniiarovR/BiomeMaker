#pragma once
#include "raylib.h"

// sizes
constexpr int FIELD_HEIGHT       = 30;
constexpr int FIELD_WIDTH        = 80;
constexpr int LABEL_WIDTH        = 180;
constexpr int BUTTON_WIDTH       = 30;
constexpr int BUTTON_SPACING     = 10;
constexpr int FIELD_SPACING      = 10;
constexpr int TEXT_SIZE          = 20;
constexpr int SECTION_TITLE_SIZE = 40;
constexpr int INITIAL_Y          = 5;
constexpr int LINE_HEIGHT        = FIELD_HEIGHT + FIELD_SPACING;
constexpr int SECTION_SPACING    = 5;

// scroll
constexpr int SCROLL_SPEED       = 40;

// colors
constexpr Color COLOR_BG_DEFAULT     = LIGHTGRAY;
constexpr Color COLOR_BG_EDITING     = SKYBLUE;
constexpr Color COLOR_TEXT           = WHITE;
constexpr Color COLOR_TEXT_FIELD     = BLACK;
constexpr Color COLOR_SECTION_TITLE  = RED;
constexpr Color COLOR_KEYBIND_BTN    = GRAY;
constexpr Color COLOR_PLUS_MINUS_BTN = DARKGRAY;

