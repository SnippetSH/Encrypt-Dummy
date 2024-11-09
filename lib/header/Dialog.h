#ifndef DIALOG_H
#define DIALOG_H

#include <windows.h>

class Dialog {
public:
    Dialog(bool isWithCancel);
    ~Dialog();
    INT_PTR ShowDialog(HWND hwndParent, const wchar_t* message);
    static INT_PTR CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    bool isWithCancel;
    static wchar_t* g_message;
};

#endif