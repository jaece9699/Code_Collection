#include <windows.h>
#include <stdio.h>

#include <windows.h>
#include <stdio.h>

#define WIDTH 9
#define HEIGHT 9
#define BASE_ADDR 0x01005361 // �� IDA���� �м��� �ּҷ� ��ü

void RightClickCell(HWND hwnd, int x, int y) {
    int cellSize = 16;

    // �� ������ ����: ������ �׵θ� �� ������ ���� ��ġ
    int boardOffsetX = 12;  // ȭ�� ���ʿ��� ���� ���۱��� ���� (���������� ã�� ��)
    int boardOffsetY = 56;  // ȭ�� ������ ���� ���۱��� ����

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

    // 1. ���� ���� �ϳ� ã�� (ex: 0x8F)
    BYTE bombValue = 0;
    bool found = false;
    int bomb_count = 0;

    
    for (int y = 0; y < HEIGHT && !found; ++y) {
        for (int x = 0; x < WIDTH && !found; ++x) {
            
            int offset = y * 0x20 + x;
            BYTE val = *(map + offset);
            bombValue = val;
            if (val == 0x8F || val == 0x8E) { // ���ڶ�� �����Ǵ� ����
                RightClickCell(hwnd, x, y);
                printf("[*] ���� ���� �߰�: 0x%02X at (%d,%d)\n", bombValue, x, y);
                bomb_count+=1;
                }
            }
        }   
    printf("Ž���� ���� ���� : %d\n", bomb_count);
    printf("[+] ��� ���� ǥ�� �Ϸ�\n");
    return 0;
}



BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason, LPVOID lpReserved) {
    if (ul_reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, FlagThread, NULL, 0, NULL);
    }
    return TRUE;
}