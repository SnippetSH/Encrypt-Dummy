#include "MainWindow.h"

PCWSTR MainWindow::ClassName() const { return L"MainWindow"; }

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HICON icon = NULL;
    static HCURSOR cursor = LoadCursor(NULL, IDC_HAND);

    if (uMsg == WM_CREATE) {
        HWND button = CreateWindowExW(
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

        icon = (HICON)LoadImageW(
            NULL,
            L"./assets/open.ico",
            IMAGE_ICON,
            100, 33,
            LR_LOADFROMFILE
        );

        if (icon == NULL) {
            MessageBoxW(NULL, L"Failed to load icon", L"Error", MB_OK);
        } else {
            SendMessageW(button, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)icon);
        }
        return 0;    
    }

    switch (uMsg) {
        case WM_CLOSE:
            if (MessageBoxW(Window(), L"정말 종료하시겠습니까?", L"종료", MB_YESNO) == IDYES) {
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

                OnSize(Window(), (UINT)wParam, w, h);
            }

            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_DRAWITEM:
            if (wParam == IDC_BUTTON1 && icon) {
                DrawButton(Window(), (LPDRAWITEMSTRUCT)lParam, icon);
            }
            return 0;
        case WM_SETCURSOR:
            if ((HWND)wParam == GetDlgItem(Window(), IDC_BUTTON1)) {
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
