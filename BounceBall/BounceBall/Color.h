#pragma once
#include <windows.h>

class Color
{
private:
	HPEN myPen, osPen;
	HBRUSH myBrush, osBrush;
public:
	void initColorScreen(HDC hdc, int red, int green, int blue);
};
