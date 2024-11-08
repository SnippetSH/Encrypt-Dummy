#include "header/MainWindow.h"


PCWSTR MainWindow::ClassName() const { return L"MainWindow"; }

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HICON icon1 = NULL;
    static HICON icon2 = NULL;
    static HCURSOR cursor = LoadCursor(NULL, IDC_HAND);

    if (uMsg == WM_CREATE) {
        RECT rect;
        GetClientRect(Window(), &rect);
        int h = rect.bottom;

        HWND buttonOpen = CreateWindowExW(
            0,
            L"button",
            L"",
            WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            10, 10, 100, 33,
            Window(),
            (HMENU)IDC_BUTTON1,
            NULL,
            NULL
        );
        HWND buttonRun = CreateWindowExW(
            0,
            L"button",
            L"",
            WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            10, h - 10 - 33, 100, 33,
            Window(),
            (HMENU)IDC_BUTTON2,
            NULL,
            NULL
        );

        icon1 = (HICON)LoadImageW(
            NULL,
            L"./assets/open.ico",
            IMAGE_ICON,
            100, 33,
            LR_LOADFROMFILE
        );

        icon2 = (HICON)LoadImageW(
            NULL,
            L"./assets/run.ico",
            IMAGE_ICON,
            100, 33,
            LR_LOADFROMFILE
        );

        if (icon1 == NULL || icon2 == NULL) {
            MessageBoxW(NULL, L"Failed to load icon", L"Error", MB_OK);
        } else {
            SendMessageW(buttonOpen, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon1);
            SendMessageW(buttonRun, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon2);
        }
        return 0;    
    }

    switch (uMsg) {
        case WM_CLOSE:
            if (ShowCustomDialogWithCancel(Window(), L"정말 종료하시겠습니까?") == IDOK) {
                DestroyWindow(Window());
            }
            return 0;
        case WM_PAINT: // 윈도우 창의 크기가 변화하거나, 가려졌다가 보이거나 하는 등의 모든 변화가 생길 때 호출
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(Window(), &ps);
            
                        
                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
                EndPaint(Window(), &ps);
            }
            return 0;
        case WM_SIZE:
            {
                int w = LOWORD(lParam);
                int h = HIWORD(lParam);

                HWND buttonRun = GetDlgItem(Window(), IDC_BUTTON2);
                SetWindowPos(buttonRun, NULL,
                    10, h - 10 - 33,
                    100, 33,
                    SWP_NOZORDER
                );

                OnSize(Window(), (UINT)wParam, w, h);
            }

            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_DRAWITEM:
            if (wParam == IDC_BUTTON1 && icon1) {
                DrawButton(Window(), (LPDRAWITEMSTRUCT)lParam, icon1);
            } else if (wParam == IDC_BUTTON2 && icon2) {
                DrawButton(Window(), (LPDRAWITEMSTRUCT)lParam, icon2);
            }
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_BUTTON1) {
                OnButtonOpen(Window(), lParam);
            } else if (LOWORD(wParam) == IDC_BUTTON2) {
                OnButtonRun(Window(), lParam);
            }
            return 0;
        case WM_SETCURSOR:
            if ((HWND)wParam == GetDlgItem(Window(), IDC_BUTTON1)) {
                SetCursor(cursor);
                return TRUE;
            } else if ((HWND)wParam == GetDlgItem(Window(), IDC_BUTTON2)) {
                SetCursor(cursor);
                return TRUE;
            }
            
        default:
            return DefWindowProc(Window(), uMsg, wParam, lParam);
    }
}

void OnSize(HWND hwnd, UINT state, int width, int height) {
    if (state == SIZE_MINIMIZED) {
        ShowWindow(hwnd, SW_MINIMIZE);
    } else if (state == SIZE_MAXIMIZED) {
        ShowWindow(hwnd, SW_MAXIMIZE);
    } else if (state == SIZE_RESTORED) {
        ShowWindow(hwnd, SW_RESTORE);
    }

    InvalidateRect(hwnd, NULL, TRUE);
}

void DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon) {
    LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lpDrawItem;

    // 배경 지우기 (투명 배경)
    FillRect(pdis->hDC, &pdis->rcItem, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // 아이콘 그리기
    if (icon) {
        int iconX = (pdis->rcItem.right - pdis->rcItem.left - 100) / 2; // 가운데 정렬
        int iconY = (pdis->rcItem.bottom - pdis->rcItem.top - 33) / 2;
        DrawIconEx(pdis->hDC, pdis->rcItem.left + iconX, pdis->rcItem.top + iconY, icon, 100, 33, 0, NULL, DI_NORMAL);
    }
}

wchar_t* g_message = NULL;

INT_PTR ShowCustomDialog(HWND hwndParent, const wchar_t* message) {
    if (g_message) {
        free(g_message);
    }

    g_message = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(message) + 1));
    wcscpy(g_message, message);
    // 리소스에서 대화상자 생성
    INT_PTR result = DialogBoxParamW(
        GetModuleHandle(NULL),
        MAKEINTRESOURCEW(IDD_CUSTOM_DIALOG),
        hwndParent,
        CustomDialogProc,
        (LPARAM)message
    );

    if (g_message != NULL) {
        free(g_message);
        g_message = NULL;
    }

    return result;
}

INT_PTR ShowCustomDialogWithCancel(HWND hwndParent, const wchar_t* message) {
    if (g_message) {
        free(g_message);
    }

    g_message = (wchar_t*)malloc(sizeof(wchar_t) * (wcslen(message) + 1));
    wcscpy(g_message, message);
    // 리소스에서 대화상자 생성
    INT_PTR result = DialogBoxParamW(
        GetModuleHandle(NULL),
        MAKEINTRESOURCEW(IDD_CUSTOM_DIALOG_WITH_CANCEL),
        hwndParent,
        CustomDialogProc,
        (LPARAM)message
    );

    if (g_message != NULL) {
        free(g_message);
        g_message = NULL;
    }

    return result;
}

INT_PTR CALLBACK CustomDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
            const wchar_t* message = g_message;
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
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
                EndDialog(hwndDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
        }
    }
    return FALSE;
}

INT_PTR CALLBACK CustomDialogWithCancelProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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
            const wchar_t* message = g_message;
            if (wcscmp((const wchar_t*)lParam, L"Open") == 0) {
                caption = message;

            } else if (wcscmp((const wchar_t*)lParam, L"Run") == 0) {
                caption = message;
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

void OnButtonOpen(HWND hwnd, LPARAM lParam) {
    ShowCustomDialog(hwnd, L"Open");
}

void OnButtonRun(HWND hwnd, LPARAM lParam) {
    ShowCustomDialog(hwnd, L"Run");
}