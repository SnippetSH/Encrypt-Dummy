#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <tchar.h>
#include "BaseWindow.h"
#include "Dialog.h"
#include "../../resources/resource.h"

class MainWindow : public BaseWindow<MainWindow> {
private:
    static wchar_t* g_fileName;
    static wchar_t* g_userKey;
    HWND m_hEditInput;

public:
    MainWindow() : m_hEditInput(NULL) {}
    PCWSTR ClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    static Dialog* GetDialog(int index);
    void OnSize(HWND hwnd, UINT state, int width, int height);
    void DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon);
    void OnButtonOpen(HWND hwnd, LPARAM lParam);
    void OnButtonRun(HWND hwnd, LPARAM lParam);
};





#endif
