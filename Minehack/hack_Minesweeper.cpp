#include "windows.h"
#include <stdio.h>
#define WIDTH 9
#define HEIGHT 9
#define BASE_ADDR 0x01005340 // IDA에서 분석한 실제 주소로 교체 필요

// 클릭 시뮬레이션
void LeftClickCell(HWND hwnd, int x, int y) {
    int cellSize = 16; // 보통 한 타일 16x16
    LPARAM lParam = MAKELPARAM(x * cellSize + 8, y * cellSize + 8); // 중앙 클릭

    PostMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    PostMessage(hwnd, WM_LBUTTONUP, 0, lParam);
}

DWORD WINAPI ThreadProc(LPVOID lp) {
    // 콘솔 열기
    AllocConsole();
    FILE* f; freopen_s(&f, "CONOUT$", "w", stdout);

    HWND hwnd = FindWindow(NULL, L"Minesweeper"); // 윈도우 제목

    BYTE* map = (BYTE*)BASE_ADDR;

    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            BYTE val = map[y * WIDTH + x];

            if (val == 0x0F) { // 닫힌 빈 칸
                printf("클릭: (%d, %d)\n", x, y);
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