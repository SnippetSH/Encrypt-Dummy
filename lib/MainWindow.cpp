#include "header/MainWindow.h"
#include "header/BaseWindow.h"
#include <iostream>

template <>
Dialog** BaseWindow<MainWindow>::m_dialog = nullptr;

wchar_t* MainWindow::g_fileName = NULL;
wchar_t* MainWindow::g_userKey = NULL;

PCWSTR MainWindow::ClassName() const { return L"MainWindow"; }
Dialog* MainWindow::GetDialog(int index) { return BaseWindow::m_dialog[index]; }

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HICON icon1 = NULL;
    static HICON icon2 = NULL;
    static HCURSOR cursor = LoadCursor(NULL, IDC_HAND);

    if (uMsg == WM_CREATE) {
        if (m_dialog == NULL) {
            m_dialog = new Dialog*[2];
            m_dialog[0] = new Dialog(false);
            m_dialog[1] = new Dialog(true);
        }

        RECT rect;
        GetClientRect(Window(), &rect);
        int h = rect.bottom;
        int w = rect.right;

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

        HWND staticFileName =CreateWindowExW(
            0,
            L"static",
            L"Selected File: None",
            WS_VISIBLE | WS_CHILD | SS_CENTER,
            w / 2 - (w - 100) / 2, h / 2 - (h - 100) / 2, w - 100, h - 100,
            Window(),
            (HMENU)IDC_STATIC_FILE_NAME,
            NULL,
            NULL
        );
        HBRUSH brush = CreateSolidBrush(RGB(82,94,152));
        SetClassLongPtrW(staticFileName, GCLP_HBRBACKGROUND, (LONG_PTR)brush);

        int editSize = 200;
        if (w > 1000) {
            editSize = 500;
        }
        m_hEditInput = CreateWindowExW(
            0,
            L"edit",
            L"Please Input Your Key.",
            WS_CHILD | ES_AUTOHSCROLL | SS_CENTER,
            w / 2 - (w - editSize) / 2, h / 2 - (50) / 2, w - editSize, 50,
            Window(),
            (HMENU)IDC_EDIT_INPUT,
            NULL,
            NULL
        );

        /*
        HFONT font = CreateFontW(
            16,                     // nHeight
            0,                      // nWidth
            0,                      // nEscapement
            0,                      // nOrientation
            FW_NORMAL,             // fnWeight
            FALSE,                 // fdwItalic
            FALSE,                 // fdwUnderline
            FALSE,                 // fdwStrikeOut
            DEFAULT_CHARSET,       // fdwCharSet
            OUT_DEFAULT_PRECIS,    // fdwOutputPrecision
            CLIP_DEFAULT_PRECIS,   // fdwClipPrecision
            DEFAULT_QUALITY,       // fdwQuality
            DEFAULT_PITCH | FF_DONTCARE,  // fdwPitchAndFamily
            L"Consolas"           // lpszFace
        );
        */
        HFONT font = CreateFontW(
            30, 0, 0, 0,
            FW_DEMIBOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, 0, 0, 0, 0,
            L"Consolas"
        );
        SendMessageW(m_hEditInput, WM_SETFONT, (WPARAM)font, TRUE);
        SetClassLongPtrW(m_hEditInput, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
        return 0;    
    }

    switch (uMsg) {
        case WM_CLOSE:
            if (GetDialog(1)->ShowDialog(Window(), L"정말 종료하시겠습니까?") == IDOK) {
                DestroyWindow(Window());
            } 
            return 0;
        case WM_PAINT: // 윈도우 창의 크기가 변화하거나, 가려졌다가 보이거나 하는 등의 모든 변화가 생길 때 호출
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(Window(), &ps);
            
                HBRUSH hBrush = CreateSolidBrush(RGB(82,94,152));
                FillRect(hdc, &ps.rcPaint, hBrush);
                DeleteObject(hBrush);

                EndPaint(Window(), &ps);
            }
            return 0;
        case WM_SIZE:
            {
                int w = LOWORD(lParam);
                int h = HIWORD(lParam);

                OnSize(Window(), (UINT)wParam, w, h);

                HWND buttonRun = GetDlgItem(Window(), IDC_BUTTON2);
                SetWindowPos(buttonRun, NULL,
                    10, h - 10 - 33,
                    100, 33,
                    SWP_NOZORDER
                );

                HWND staticFileName = GetDlgItem(Window(), IDC_STATIC_FILE_NAME);
                SetWindowPos(staticFileName, NULL,
                    w / 2 - (w - 100) / 2, h / 2 - (h - 100) / 2, 
                    w - 100, h - 100,
                    SWP_NOZORDER
                );

                int editSize = 200;
                if (w > 1000) {
                    editSize = 500;
                }
                HWND editInput = GetDlgItem(Window(), IDC_EDIT_INPUT);
                SetWindowPos(editInput, NULL,
                    w / 2 - (w - editSize) / 2, h / 2 - (50) / 2,
                    w - editSize, 50,
                    SWP_NOZORDER
                );
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
            } else if (LOWORD(wParam) == IDC_EDIT_INPUT && HIWORD(wParam) == EN_CHANGE) {
                wchar_t buffer[MAX_PATH] = {};
                GetWindowTextW(GetDlgItem(Window(), IDC_EDIT_INPUT), buffer, MAX_PATH);
                g_userKey = buffer;
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
        case WM_CTLCOLORSTATIC: 
            {
                HDC hdcStatic = (HDC)wParam;
                HWND hwndStatic = (HWND)lParam;

                if (GetDlgCtrlID(hwndStatic) == IDC_STATIC_FILE_NAME) {
                    SetTextColor(hdcStatic, RGB(244,247,233));     // 텍스트 색상
                    SetBkColor(hdcStatic, RGB(82,94,152)); // 배경 색상
                    static HBRUSH hBrush = CreateSolidBrush(RGB(82,94,152));
                    return (LRESULT)hBrush;
                }

                return DefWindowProc(Window(), uMsg, wParam, lParam);
            }
        case WM_CTLCOLOREDIT:
            {
                HDC hdcEdit = (HDC)wParam;
                HWND hwndEdit = (HWND)lParam;
                
                if (GetDlgCtrlID(hwndEdit) == IDC_EDIT_INPUT) {
                    SetTextColor(hdcEdit, RGB(244,247,233));     // 텍스트 색상
                    SetBkColor(hdcEdit, RGB(51,59,97)); // 배경 색상
                    static HBRUSH hBrush = CreateSolidBrush(RGB(51,59,97));
                    return (LRESULT)hBrush;
                }

                return DefWindowProc(Window(), uMsg, wParam, lParam);
            }
        case WM_GETMINMAXINFO:
            {
                LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
                lpMMI->ptMinTrackSize.x = 640;  // 최소 너비
                lpMMI->ptMinTrackSize.y = 480;  // 최소 높이
                return 0;
            }
        default:
            return DefWindowProc(Window(), uMsg, wParam, lParam);
    }
}

void MainWindow::OnSize(HWND hwnd, UINT state, int width, int height) {
    if (width < 640 || height < 480) {
        return;
    }

    if (state == SIZE_MINIMIZED) {
        ShowWindow(hwnd, SW_MINIMIZE);
    } else if (state == SIZE_MAXIMIZED) {
        ShowWindow(hwnd, SW_MAXIMIZE);
    } else if (state == SIZE_RESTORED) {
        ShowWindow(hwnd, SW_RESTORE);
    }

    InvalidateRect(hwnd, NULL, TRUE);
}

void MainWindow::DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon) {
    LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lpDrawItem;

    // 배경 지우기 (투명 배경)
    HBRUSH hBrush = CreateSolidBrush(RGB(82,94,152));
    FillRect(pdis->hDC, &pdis->rcItem, hBrush);
    DeleteObject(hBrush);

    // 아이콘 그리기
    if (icon) {
        int iconX = (pdis->rcItem.right - pdis->rcItem.left - 100) / 2; // 가운데 정렬
        int iconY = (pdis->rcItem.bottom - pdis->rcItem.top - 33) / 2;
        DrawIconEx(pdis->hDC, pdis->rcItem.left + iconX, pdis->rcItem.top + iconY, icon, 100, 33, 0, NULL, DI_NORMAL);
    }
}

void MainWindow::OnButtonOpen(HWND hwnd, LPARAM lParam) {
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = {};
    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"All Files\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrInitialDir = L"./";

    if (GetOpenFileNameW(&ofn)) {
        GetDialog(0)->ShowDialog(hwnd, fileName);
        g_fileName = fileName;

        wchar_t displayFileName[MAX_PATH + 50] = L"Selected File: ";
        wchar_t tempFileName[MAX_PATH] = {};
        int cnt = 0, tempCnt = 0;

        for (int i = wcslen(fileName); i > 0; i--) {
            std::wcout << fileName[i] << " ";
            if (fileName[i] == '\\') {
                cnt++;

                if (cnt == 2) {
                    break;
                }
            }
            tempFileName[tempCnt++] = fileName[i];
        }

        wchar_t finalFileName[MAX_PATH] = {};
        for (int i = 0; i < tempCnt; i++) {
            finalFileName[i] = tempFileName[tempCnt - i - 1];
        }

        RECT rect;
        GetClientRect(Window(), &rect);
        int h = rect.bottom;
        int w = rect.right;
        std::cout << "w: " << w << " h: " << h << "\n";

        ShowWindow(m_hEditInput, SW_SHOW);
        SetFocus(m_hEditInput);

        int editSize = 200;
        if (w > 1000) {
            editSize = 500;
        }
        SetWindowPos(  
            m_hEditInput, NULL, w / 2 - (w - editSize) / 2, h / 2 - (50) / 2, w - editSize, 50,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER
        );
        wcscat_s(displayFileName, finalFileName);
        SetWindowTextW(GetDlgItem(hwnd, IDC_STATIC_FILE_NAME), displayFileName);
    }
}

void MainWindow::OnButtonRun(HWND hwnd, LPARAM lParam) {
    GetDialog(0)->ShowDialog(hwnd, L"Run");
}