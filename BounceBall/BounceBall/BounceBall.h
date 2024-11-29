#pragma once

#include "resource.h"
#include <windows.h>
#include "Game.h"
#include "Color.h"
#include <tchar.h>

enum ScreenMode {
    TITLE_SCREEN,
    GAME_SCREEN,
    COLOR_SCREEN,
    PAUSE_SCREEN
};

Game game;
Color color;
HBITMAP hPauseImage;
HBRUSH bgBrush;
static HDC imgDC, exitDC;
static HDC memDC;
static HBITMAP memBitmap, myBitmap;
static ScreenMode currentScreen = TITLE_SCREEN;
static int red = 255, green = 235, blue = 0;
static bool isLeftPressed = false;
static bool isRightPressed = false;
int background = 0;