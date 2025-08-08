#include "windows.h"
#include <stdio.h>
#define WIDTH 9
#define HEIGHT 9
#define BASE_ADDR 0x01005340 // IDA���� �м��� ���� �ּҷ� ��ü �ʿ�

// Ŭ�� �ùķ��̼�
void LeftClickCell(HWND hwnd, int x, int y) {
    int cellSize = 16; // ���� �� Ÿ�� 16x16
    LPARAM lParam = MAKELPARAM(x * cellSize + 8, y * cellSize + 8); // �߾� Ŭ��

    PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    PostMessage(hwnd, WM_LBUTTONUP, 0, lParam);
}

DWORD WINAPI ThreadProc(LPVOID lp) {
    // �ܼ� ����
    AllocConsole();
    FILE* f; freopen_s(&f, "CONOUT$", "w", stdout);

    HWND hwnd = FindWindow(NULL, L"Minesweeper"); // ������ ����

    BYTE* map = (BYTE*)BASE_ADDR;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            BYTE val = map[y * WIDTH + x];

            if (val == 0x0F) { // ���� �� ĭ
                printf("Ŭ��: (%d, %d)\n", x, y);
                LeftClickCell(hwnd, x, y);
                Sleep(10);
            }
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(0, 0, ThreadProc, 0, 0, 0);
    }
    return TRUE;
}