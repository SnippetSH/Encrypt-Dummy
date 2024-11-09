#ifndef ENTRY_H
#define ENTRY_H

#include "c_entry.h"
#include <windows.h>
#include "MainWindow.h"

extern HINSTANCE g_instance;
// Windows API 함수는 내부적으로만 사용하므로 extern "C"에서 제외
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

#endif
