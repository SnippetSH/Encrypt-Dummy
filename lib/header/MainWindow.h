#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define IDC_BUTTON1 1001
#define IDC_BUTTON2 1002


#include <tchar.h>
#include "BaseWindow.h"
#include "../../resources/resource.h"

class MainWindow : public BaseWindow<MainWindow> {
public:
    PCWSTR ClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

void OnSize(HWND hwnd, UINT state, int width, int height);
void DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon);
void OnButtonOpen(HWND hwnd, LPARAM lParam);
void OnButtonRun(HWND hwnd, LPARAM lParam);

INT_PTR ShowCustomDialog(HWND hwndParent, const wchar_t* message);
INT_PTR ShowCustomDialogWithCancel(HWND hwndParent, const wchar_t* message);

INT_PTR CALLBACK CustomDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK CustomDialogWithCancelProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
