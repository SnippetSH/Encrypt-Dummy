#include "lib/MainWindow.h"
#include "entry.h"
#include <iostream>

using namespace std;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    MainWindow wnd;

    if (!wnd.Create(L"Window", WS_OVERLAPPEDWINDOW)) {
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
    return wWinMain(GetModuleHandle(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}
