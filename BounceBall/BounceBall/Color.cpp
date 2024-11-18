#include "Color.h"

void InitColorScreen(HWND hWnd, HDC hdc) {
    // 색 변경 화면을 그리거나 로직 처리
    TextOut(hdc, 100, 100, TEXT("색 변경 화면"), lstrlen(TEXT("색 변경 화면")));
}
