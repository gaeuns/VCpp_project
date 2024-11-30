#pragma once

#include "Resource.h"
#include <windows.h>
#include "Game.h"
#include "Color.h"
#include <tchar.h>

enum ScreenMode {
    TITLE_SCREEN,
    GAME_SCREEN,
    COLOR_SCREEN
};

Game game;
Color color;

HDC imgDC;
HDC memDC;
HBRUSH bgBrush;
HICON hIcon;
HFONT hFont, oldFont;
HBITMAP memBitmap, myBitmap;

ScreenMode currentScreen = TITLE_SCREEN;

int red = 255, green = 235, blue = 0;
bool isLeftPressed = false;
bool isRightPressed = false;
int background = 0;
