#include "header/Dialog.h"
#include "../resources/resource.h"

extern HINSTANCE g_instance;

wchar_t* Dialog::g_message = NULL;
Dialog::Dialog(bool isWithCancel) : isWithCancel(isWithCancel) {}
Dialog::~Dialog() {}

INT_PTR Dialog::ShowDialog(HWND hwndParent, const wchar_t* message) {
    if (g_message) {
        free(g_message);
    }

    g_message = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(message) + 1));
    wcscpy(g_message, message);
    // 리소스에서 대화상자 생성
    INT_PTR result = DialogBoxParamW(
        g_instance,
        MAKEINTRESOURCEW(isWithCancel ? IDD_CUSTOM_DIALOG_WITH_CANCEL : IDD_CUSTOM_DIALOG),
        hwndParent,
        DialogProc,
        (LPARAM)message
    );

    if (g_message != NULL) {
        free(g_message);
        g_message = NULL;
    }

    return result;
}

INT_PTR CALLBACK Dialog::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_INITDIALOG: {
            // 대화 상자가 초기화될 때 호출됩니다.
            // 대화 상자를 화면 가운데로 이동시키는 예시 코드입니다.
            RECT rcOwner, rcDlg; 
            GetWindowRect(GetParent(hwndDlg), &rcOwner);
            GetWindowRect(hwndDlg, &rcDlg);

            int ownerWidth = rcOwner.right - rcOwner.left;
            int ownerHeight = rcOwner.bottom - rcOwner.top;
            int dlgWidth = rcDlg.right - rcDlg.left;
            int dlgHeight = rcDlg.bottom - rcDlg.top;

            int x = rcOwner.left + (ownerWidth - dlgWidth) / 2;
            int y = rcOwner.top + (ownerHeight - dlgHeight) / 2;

            SetWindowPos(hwndDlg, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

            const wchar_t* caption = L"Dialog";
            const wchar_t* message = Dialog::g_message;
            if (wcscmp((const wchar_t*)lParam, L"Open") == 0) {
                caption = L"Open";

            } else if (wcscmp((const wchar_t*)lParam, L"Run") == 0) {
                caption = L"Run";
            }
            SetWindowTextW(hwndDlg, caption);
            SetDlgItemTextW(hwndDlg, IDC_STATIC_TEXT, message);
            return TRUE;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK:
                    EndDialog(hwndDlg, IDOK);
                    break;
                case IDCANCEL:
                    EndDialog(hwndDlg, IDCANCEL);
                    break;
            }
            break;
        }
    }
    return FALSE;
}