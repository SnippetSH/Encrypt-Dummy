#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define IDC_BUTTON1 1001

#include "BaseWindow.h"

class MainWindow : public BaseWindow<MainWindow> {
public:
    PCWSTR ClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

void OnSize(HWND hwnd, UINT state, int width, int height);
void DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon);

#endif
