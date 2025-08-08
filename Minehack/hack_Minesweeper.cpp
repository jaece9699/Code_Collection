#include <windows.h>
#include <stdio.h>

#include <windows.h>
#include <stdio.h>

#define WIDTH 9
#define HEIGHT 9
#define BASE_ADDR 0x01005361 // ← IDA에서 분석한 주소로 교체

void RightClickCell(HWND hwnd, int x, int y) {
    int cellSize = 16;

    // ★ 오프셋 보정: 윈도우 테두리 및 보드판 시작 위치
    int boardOffsetX = 12;  // 화면 왼쪽에서 보드 시작까지 여백 (실험적으로 찾은 값)
    int boardOffsetY = 56;  // 화면 위에서 보드 시작까지 여백

    LPARAM lParam = MAKELPARAM(boardOffsetX + x * cellSize + cellSize / 2,
        boardOffsetY + y * cellSize + cellSize / 2);

    PostMessage(hwnd, WM_RBUTTONDOWN, MK_RBUTTON, lParam);
    PostMessage(hwnd, WM_RBUTTONUP, 0, lParam);
}

DWORD WINAPI FlagThread(LPVOID lp) {
    AllocConsole();
    FILE* f; freopen_s(&f, "CONOUT$", "w", stdout);

    HWND hwnd = FindWindow(NULL, L"Minesweeper");
    BYTE* map = (BYTE*)BASE_ADDR;

    // 1. 지뢰 패턴 하나 찾기 (ex: 0x8F)
    BYTE bombValue = 0;
    bool found = false;
    int bomb_count = 0;

    
    for (int y = 0; y < HEIGHT && !found; ++y) {
        for (int x = 0; x < WIDTH && !found; ++x) {
            
            int offset = y * 0x20 + x;
            BYTE val = *(map + offset);
            bombValue = val;
            if (val == 0x8F || val == 0x8E) { // 지뢰라고 추정되는 패턴
                RightClickCell(hwnd, x, y);
                printf("[*] 지뢰 패턴 발견: 0x%02X at (%d,%d)\n", bombValue, x, y);
                bomb_count+=1;
                }
            }
        }   
    printf("탐지된 지뢰 갯수 : %d\n", bomb_count);
    printf("[+] 깃발 전부 표시 완료\n");
    return 0;
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason, LPVOID lpReserved) {
    if (ul_reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, FlagThread, NULL, 0, NULL);
    }
    return TRUE;
}