#include "Color.h"

void InitColorScreen(HWND hWnd, HDC hdc) {
    // �� ���� ȭ���� �׸��ų� ���� ó��
    TextOut(hdc, 100, 100, TEXT("�� ���� ȭ��"), lstrlen(TEXT("�� ���� ȭ��")));
}
