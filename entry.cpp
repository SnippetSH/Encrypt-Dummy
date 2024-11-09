#include "lib/header/entry.h"
#include <iostream>

using namespace std;

HINSTANCE g_instance = NULL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    MainWindow wnd;

    if (!wnd.Create(L"Window", WS_OVERLAPPEDWINDOW, 0, 0, 640, 480)) {
        cout << "CreateWindowEx failed: " << GetLastError() << "\n";
        return 0;
    }

    ShowWindow(wnd.Window(), nCmdShow);

    MSG msg = {};

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

int startApp() {
    g_instance = GetModuleHandle(NULL);
    return wWinMain(g_instance, NULL, GetCommandLineW(), SW_SHOWNORMAL);
}
