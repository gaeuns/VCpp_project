#include "Color.h"

void Color::initColorScreen(HDC hdc, int red, int green, int blue)
{
    myPen = CreatePen(PS_SOLID, 1, RGB(red, green, blue));
    myBrush = CreateSolidBrush(RGB(red, green, blue));
    osPen = (HPEN)SelectObject(hdc, myPen);
    osBrush = (HBRUSH)SelectObject(hdc, myBrush);

    Ellipse(hdc, 100, 225, 200, 325);

    SelectObject(hdc, osPen);
    SelectObject(hdc, osBrush);
    DeleteObject(myPen);
    DeleteObject(myBrush);
}
