#pragma once

#include "resource.h"
#include <windows.h>
#include "Game.h"
#include "Color.h"

enum ScreenMode {
    TITLE_SCREEN,
    GAME_SCREEN,
    COLOR_SCREEN
};

bool isLeftPressed = false;
bool isRightPressed = false;