#pragma once

#include "resource.h"
#include <windows.h>
#include "Game.h"
#include "Color.h"

enum ScreenMode {
    TITLE_SCREEN,
    GAME_SCREEN,
    COLOR_SCREEN,
    PAUSE_SCREEN
};

static ScreenMode currentScreen = TITLE_SCREEN;
static bool isLeftPressed = false;
static bool isRightPressed = false;