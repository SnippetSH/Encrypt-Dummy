#include "header/CryptoWindow.h"
#include "header/BaseWindow.h"
#include <iostream>

template <>
Dialog** BaseWindow<CryptoWindow>::m_dialog = nullptr;

wchar_t* CryptoWindow::g_fileName = NULL;
wchar_t* CryptoWindow::g_userKey = NULL;

CryptoWindow::CryptoWindow()
    : m_hEditInput(NULL), m_hStaticUserKeyLength(NULL),
      m_hButtonOpen(NULL), m_hButtonEncrypt(NULL), m_hButtonDecrypt(NULL),
      m_hStaticFileName(NULL), m_iconOpen(NULL), m_iconEncrypt(NULL),
      m_iconDecrypt(NULL), m_cursorHand(NULL) {}

PCWSTR CryptoWindow::ClassName() const { return L"CryptoWindow"; }
Dialog* CryptoWindow::GetDialog(int index) { return BaseWindow::m_dialog[index]; }

LRESULT CryptoWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            return OnCreate();
        case WM_CLOSE:
            return OnClose();
        case WM_PAINT:
            return OnPaint();
        case WM_SIZE:
            return OnSize(wParam, lParam);
        case WM_DESTROY:
            return OnDestroy();
        case WM_DRAWITEM:
            return OnDrawItem(wParam, lParam);
        case WM_COMMAND:
            return OnCommand(wParam, lParam);
        case WM_SETCURSOR:
            return OnSetCursor(wParam, lParam);
        case WM_CTLCOLORSTATIC:
            return OnCtlColorStatic(wParam, lParam);
        case WM_CTLCOLOREDIT:
            return OnCtlColorEdit(wParam, lParam);
        case WM_GETMINMAXINFO:
            return OnGetMinMaxInfo(lParam);
        default:
            return DefWindowProc(Window(), uMsg, wParam, lParam);
    }
}

LRESULT CryptoWindow::OnCreate() {
    if (m_dialog == NULL) {
        m_dialog = new Dialog*[2];
        m_dialog[0] = new Dialog(false);
        m_dialog[1] = new Dialog(true);
    }

    LoadIcons();
    CreateControls();

    return 0;
}

void CryptoWindow::LoadIcons() {
    m_iconOpen = (HICON)LoadImageW(
        NULL,
        L"./assets/open.ico",
        IMAGE_ICON,
        81, 27,
        LR_LOADFROMFILE
    );

    m_iconEncrypt = (HICON)LoadImageW(
        NULL,
        L"./assets/encrypt.ico",
        IMAGE_ICON,
        81, 27,
        LR_LOADFROMFILE
    );

    m_iconDecrypt = (HICON)LoadImageW(
        NULL,
        L"./assets/decrypt.ico",
        IMAGE_ICON,
        81, 27,
        LR_LOADFROMFILE
    );

    m_cursorHand = LoadCursor(NULL, IDC_HAND);

    if (m_iconOpen == NULL || m_iconEncrypt == NULL || m_iconDecrypt == NULL) {
        MessageBoxW(NULL, L"Failed to load icons", L"Error", MB_OK);
    }
}

void CryptoWindow::CreateControls() {
    RECT rect;
    GetClientRect(Window(), &rect);
    int h = rect.bottom;
    int w = rect.right;

    m_hButtonOpen = CreateWindowExW(
        0,
        L"button",
        L"",
        WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        10, 10, 81, 27,
        Window(),
        (HMENU)IDC_BUTTON1,
        NULL,
        NULL
    );

    m_hButtonEncrypt = CreateWindowExW(
        0,
        L"button",
        L"",
        WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        100, 10, 81, 27,
        Window(),
        (HMENU)IDC_BUTTON2,
        NULL,
        NULL
    );

    m_hButtonDecrypt = CreateWindowExW(
        0,
        L"button",
        L"",
        WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        190, 10, 81, 27,
        Window(),
        (HMENU)IDC_BUTTON3,
        NULL,
        NULL
    );

    if (m_iconOpen && m_iconEncrypt && m_iconDecrypt) {
        SendMessageW(m_hButtonOpen, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)m_iconOpen);
        SendMessageW(m_hButtonEncrypt, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)m_iconEncrypt);
        SendMessageW(m_hButtonDecrypt, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)m_iconDecrypt);
    }

    m_hStaticFileName = CreateWindowExW(
        0,
        L"static",
        L"Selected File: None",
        WS_VISIBLE | WS_CHILD | SS_CENTER,
        w / 2 - (w - 100) / 2, 50, w - 100, 40,
        Window(),
        (HMENU)IDC_STATIC_FILE_NAME,
        NULL,
        NULL
    );

    HBRUSH brush = CreateSolidBrush(BG_COLOR);
    SetClassLongPtrW(m_hStaticFileName, GCLP_HBRBACKGROUND, (LONG_PTR)brush);

    int height = 0;
    RECT rect1;
    if (GetWindowRect(m_hStaticFileName, &rect1)) {
        POINT topLeft = { rect1.left, rect1.top };
        ScreenToClient(Window(), &topLeft);
        height = topLeft.y;
    }

    m_hStaticUserKeyLength = CreateWindowExW(
        0,
        L"static",
        L"",
        WS_CHILD | SS_CENTER,
        w / 2 - (w - 100) / 2, height + 60, w - 100, 40,
        Window(),
        (HMENU)IDC_STATIC_USER_KEY_LENGTH,
        NULL,
        NULL
    );

    SetClassLongPtrW(m_hStaticUserKeyLength, GCLP_HBRBACKGROUND, (LONG_PTR)brush);

    int editSize = w / 2;
    m_hEditInput = CreateWindowExW(
        0,
        L"edit",
        L"Please Input Your Key.",
        WS_CHILD | ES_AUTOHSCROLL | SS_CENTER,
        w / 2 - (editSize) / 2, 180,
        editSize, 50,
        Window(),
        (HMENU)IDC_EDIT_INPUT,
        NULL,
        NULL
    );

    HFONT fontEdit = CreateFontW(
        30, 0, 0, 0,
        FW_DEMIBOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, 0, 0, 0, 0,
        L"Consolas"
    );
    SendMessageW(m_hEditInput, WM_SETFONT, (WPARAM)fontEdit, TRUE);
    SetClassLongPtrW(m_hEditInput, GCLP_HBRBACKGROUND, (LONG_PTR)brush);

    DeleteObject(brush);
}

void CryptoWindow::AdjustControlPositions(int w, int h) {
    SetWindowPos(m_hStaticFileName, NULL,
        w / 2 - (w - 100) / 2, 50,
        w - 100, 40,
        SWP_NOZORDER
    );

    SetWindowPos(m_hStaticUserKeyLength, NULL,
        w / 2 - (w - 100) / 2, 110,
        w - 100, 40,
        SWP_NOZORDER
    );

    int editSize = w / 2;
    SetWindowPos(m_hEditInput, NULL,
        w / 2 - (editSize) / 2, 180,
        editSize, 50,
        SWP_NOZORDER
    );

    SetWindowPos(m_hButtonOpen, NULL, 10, 10, 81, 27, SWP_NOZORDER);
    SetWindowPos(m_hButtonEncrypt, NULL, 105, 10, 81, 27, SWP_NOZORDER);
    SetWindowPos(m_hButtonDecrypt, NULL, 200, 10, 81, 27, SWP_NOZORDER);
}

LRESULT CryptoWindow::OnSize(WPARAM wParam, LPARAM lParam) {
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    OnSizeHandle(Window(), (UINT)wParam, width, height);

    AdjustControlPositions(width, height);

    return 0;
}

void CryptoWindow::OnSizeHandle(HWND hwnd, UINT state, int width, int height) {
    if (width < 640 || height < 480) {
        return;
    }

    if (state == SIZE_RESTORED) {
        ShowWindow(hwnd, SW_RESTORE);
    }

    InvalidateRect(hwnd, NULL, TRUE);  
}

LRESULT CryptoWindow::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(Window(), &ps);

    HBRUSH hBrush = CreateSolidBrush(BG_COLOR);
    FillRect(hdc, &ps.rcPaint, hBrush);
    DeleteObject(hBrush);

    EndPaint(Window(), &ps);
    return 0;
}

LRESULT CryptoWindow::OnClose() {
    if (GetDialog(1)->ShowDialog(Window(), L"정말 종료하시겠습니까?") == IDOK) {
        DestroyWindow(Window());
    } 
    return 0;
}

LRESULT CryptoWindow::OnDestroy() {
    if (g_fileName != NULL) {
        delete[] g_fileName;
        g_fileName = NULL;
    }

    if (g_userKey != NULL) {
        delete[] g_userKey;
        g_userKey = NULL;
    }

    if (m_dialog != NULL) {
        delete m_dialog[0];
        delete m_dialog[1];
        delete[] m_dialog;
        m_dialog = NULL;
    }

    PostQuitMessage(-1);
    return -1;
}

LRESULT CryptoWindow::OnDrawItem(WPARAM wParam, LPARAM lParam) {
    if (wParam == IDC_BUTTON1 && m_iconOpen) {
        DrawButton(Window(), (LPDRAWITEMSTRUCT)lParam, m_iconOpen);
    } else if (wParam == IDC_BUTTON2 && m_iconEncrypt) {
        DrawButton(Window(), (LPDRAWITEMSTRUCT)lParam, m_iconEncrypt);
    } else if (wParam == IDC_BUTTON3 && m_iconDecrypt) {
        DrawButton(Window(), (LPDRAWITEMSTRUCT)lParam, m_iconDecrypt);
    }
    return 0;
}

LRESULT CryptoWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
    switch (LOWORD(wParam)) {
        case IDC_BUTTON1:
            OnButtonOpen(Window(), lParam);
            break;
        case IDC_BUTTON2:
            if (g_fileName != NULL && g_userKey != NULL) {
                OnButtonRun(Window(), lParam, true);
            }
            break;
        case IDC_BUTTON3:
            if (g_fileName != NULL && g_userKey != NULL) {
                OnButtonRun(Window(), lParam, false);
            }
            break;
        case IDC_EDIT_INPUT:
            if (HIWORD(wParam) == EN_CHANGE) {
                wchar_t buffer[MAX_PATH] = {};
                GetWindowTextW(m_hEditInput, buffer, MAX_PATH);
                if (g_userKey != NULL) {
                    delete[] g_userKey;
                }
                g_userKey = new wchar_t[wcslen(buffer) + 1];
                wcscpy_s(g_userKey, wcslen(buffer) + 1, buffer);

                wchar_t userKeyLength[10] = {};
                int userKeyLengthInt = wcslen(g_userKey);
                wcscpy_s(userKeyLength, 10, std::to_wstring(userKeyLengthInt).c_str());
                wchar_t message[MAX_PATH + 50] = L"Current User Key's length: ";
                wcscat_s(message, userKeyLength);
                SetWindowTextW(m_hStaticUserKeyLength, message);
            }
            break;
        default:
            return DefWindowProc(Window(), WM_COMMAND, wParam, lParam);
    }
    return 0;
}

LRESULT CryptoWindow::OnSetCursor(WPARAM wParam, LPARAM lParam) {
    HWND hWndCursor = (HWND)wParam;

    if (hWndCursor == m_hButtonOpen || hWndCursor == m_hButtonEncrypt || hWndCursor == m_hButtonDecrypt) {
        SetCursor(m_cursorHand);
        return TRUE;
    }
    return DefWindowProc(Window(), WM_SETCURSOR, wParam, lParam);
}

LRESULT CryptoWindow::OnCtlColorStatic(WPARAM wParam, LPARAM lParam) {
    HDC hdcStatic = (HDC)wParam;
    HWND hwndStatic = (HWND)lParam;

    if (hwndStatic == m_hStaticFileName || hwndStatic == m_hStaticUserKeyLength) {
        SetTextColor(hdcStatic, TEXT_COLOR);
        SetBkColor(hdcStatic, BG_COLOR);
        static HBRUSH hBrush = CreateSolidBrush(BG_COLOR);
        return (LRESULT)hBrush;
    }
    return DefWindowProc(Window(), WM_CTLCOLORSTATIC, wParam, lParam);
}

LRESULT CryptoWindow::OnCtlColorEdit(WPARAM wParam, LPARAM lParam) {
    HDC hdcEdit = (HDC)wParam;
    HWND hwndEdit = (HWND)lParam;
    
    if (hwndEdit == m_hEditInput) {
        SetTextColor(hdcEdit, TEXT_COLOR);
        SetBkColor(hdcEdit, EDIT_COLOR);
        static HBRUSH hBrush = CreateSolidBrush(EDIT_COLOR);
        return (LRESULT)hBrush;
    }
    return DefWindowProc(Window(), WM_CTLCOLOREDIT, wParam, lParam);
}

LRESULT CryptoWindow::OnGetMinMaxInfo(LPARAM lParam) {
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
    lpMMI->ptMinTrackSize.x = 640;
    lpMMI->ptMinTrackSize.y = 480;
    lpMMI->ptMaxTrackSize.x = 640;
    lpMMI->ptMaxTrackSize.y = 480;
    return 0;
}

int CryptoWindow::GetNextWindowCode() {
    return -1;
}

void CryptoWindow::DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon) {
    LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lpDrawItem;

    HBRUSH hBrush = CreateSolidBrush(BG_COLOR);
    FillRect(pdis->hDC, &pdis->rcItem, hBrush);
    DeleteObject(hBrush);

    if (icon) {
        int iconX = (pdis->rcItem.right - pdis->rcItem.left - 81) / 2;
        int iconY = (pdis->rcItem.bottom - pdis->rcItem.top - 27) / 2;
        DrawIconEx(pdis->hDC, pdis->rcItem.left + iconX, pdis->rcItem.top + iconY, icon, 81, 27, 0, NULL, DI_NORMAL);
    }
}

void CryptoWindow::OnButtonOpen(HWND hwnd, LPARAM lParam) {
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

        if (g_fileName != NULL) {
            delete[] g_fileName;
        }
        g_fileName = new wchar_t[wcslen(fileName) + 1];
        wcscpy_s(g_fileName, wcslen(fileName) + 1, fileName);

        wchar_t displayFileName[MAX_PATH + 50] = L"Selected File: ";
        wchar_t* finalFileName = makeFileName(fileName);
        wcscat_s(displayFileName, finalFileName);
        SetWindowTextW(m_hStaticFileName, displayFileName);

        delete[] finalFileName;

        ShowWindow(m_hEditInput, SW_SHOW);
        SetFocus(m_hEditInput);
        ShowWindow(m_hStaticUserKeyLength, SW_SHOW);
        SetWindowTextW(m_hStaticUserKeyLength, L"User Key's length must be 32");
    }
}

void CryptoWindow::OnButtonRun(HWND hwnd, LPARAM lParam, bool isEncrypt) {
    if (g_fileName == NULL || g_userKey == NULL) {
        return;
    }

    HMODULE hModule = LoadLibraryW(L"./bin/logic.dll");
    if (hModule == NULL) {
        GetDialog(0)->ShowDialog(hwnd, L"Failed to load logic.dll Please install logic.dll");
        return;
    }

    typedef char* (*EncryptFunc)(const char*, const char*);
    typedef char* (*DecryptFunc)(const char*, const char*);
    typedef char* (*WcharToCharFunc)(wchar_t*);
    EncryptFunc Encrypt = (EncryptFunc)GetProcAddress(hModule, "Encrypt");
    DecryptFunc Decrypt = (DecryptFunc)GetProcAddress(hModule, "Decrypt");
    WcharToCharFunc WcharToChar = (WcharToCharFunc)GetProcAddress(hModule, "WcharToChar");

    if (Encrypt == NULL || WcharToChar == NULL || Decrypt == NULL) {
        GetDialog(0)->ShowDialog(hwnd, L"Failed to get Encrypt or WcharToChar or Decrypt function");
        FreeLibrary(hModule);
        return;
    }

    char* fileName = WcharToChar(g_fileName);
    char* userKey = WcharToChar(g_userKey);

    std::string userKeyStr = userKey;

    if (userKeyStr.length() < 32) {
        userKeyStr.append(32 - userKeyStr.length(), '0');
    } else if (userKeyStr.length() > 32) {
        GetDialog(0)->ShowDialog(hwnd, L"User Key's length must be 32");
        free(fileName);
        free(userKey);
        FreeLibrary(hModule);
        return;
    }

    free(userKey);
    userKey = (char*)malloc(33);
    memcpy(userKey, userKeyStr.c_str(), 32);
    userKey[32] = '\0';

    char* result = NULL;
    if (isEncrypt) {
        result = Encrypt(fileName, userKey);
    } else {
        result = Decrypt(fileName, userKey);
    }

    if (result == NULL) {
        wchar_t message[MAX_PATH + 50] = {};
        wcscpy_s(message, isEncrypt ? L"Success to Encrypt your File: " : L"Success to Decrypt your File: ");
        wchar_t* finalFileName = makeFileName(g_fileName);
        wcscat_s(message, finalFileName);
        GetDialog(0)->ShowDialog(hwnd, message);
        delete[] finalFileName;
    } else {
        wchar_t message[MAX_PATH + 50] = {};
        wcscpy_s(message, isEncrypt ? L"Failed to Encrypt your File: " : L"Failed to Decrypt your File: ");
        wcscat_s(message, g_fileName);
        GetDialog(0)->ShowDialog(hwnd, message);
    }

    free(fileName);
    free(userKey);
    FreeLibrary(hModule);
}

wchar_t* CryptoWindow::makeFileName(wchar_t* fileName) {
    wchar_t tempFileName[MAX_PATH] = {};
    int cnt = 0, tempCnt = 0;

    for (int i = wcslen(fileName) - 1; i >= 0; i--) {
        if (fileName[i] == '\\') {
            cnt++;
            if (cnt == 2) {
                break;
            }
        }
        tempFileName[tempCnt++] = fileName[i];
    }

    wchar_t* finalFileName = new wchar_t[tempCnt + 1];
    for (int i = 0; i < tempCnt; i++) {
        finalFileName[i] = tempFileName[tempCnt - i - 1];
    }
    finalFileName[tempCnt] = L'\0';

    return finalFileName;
}
