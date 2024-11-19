#include "header/MainWindow.h"
#include "header/BaseWindow.h"

template <>
Dialog** BaseWindow<MainWindow>::m_dialog = nullptr;

MainWindow::MainWindow() : m_hSwitchToCrypto(NULL), m_iconSwitchToCrypto(NULL), m_cursorHand(NULL), m_nextWindowCode(0) {}

PCWSTR MainWindow::ClassName() const { return L"MainWindow"; }
Dialog* MainWindow::GetDialog(int index) { return BaseWindow::m_dialog[index]; }

int MainWindow::GetNextWindowCode() {
    return m_nextWindowCode;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            return OnCreate();
        case WM_CLOSE:
            return OnClose();
        case WM_DESTROY:
            return OnDestroy();
        case WM_PAINT:
            return OnPaint();
        case WM_COMMAND:
            return OnCommand(wParam, lParam);
        case WM_SIZE:
            return OnSize(wParam, lParam);
        case WM_DRAWITEM:
            return OnDrawItem(wParam, lParam);
        case WM_GETMINMAXINFO:
            return OnGetMinMaxInfo(lParam);
        case WM_SETCURSOR:
            return OnSetCursor(wParam, lParam);
        default:
            return DefWindowProc(Window(), uMsg, wParam, lParam);
    }
}

LRESULT MainWindow::OnGetMinMaxInfo(LPARAM lParam) {
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
    lpMMI->ptMinTrackSize.x = 640;
    lpMMI->ptMinTrackSize.y = 480;
    lpMMI->ptMaxTrackSize.x = 640;
    lpMMI->ptMaxTrackSize.y = 480;
    return 0;
}

LRESULT MainWindow::OnSetCursor(WPARAM wParam, LPARAM lParam) {
    HWND hWndCursor = (HWND)wParam;

    if (hWndCursor == m_hSwitchToCrypto) {
        SetCursor(m_cursorHand);
        return TRUE;
    }
    return DefWindowProc(Window(), WM_SETCURSOR, wParam, lParam);
}

LRESULT MainWindow::OnCreate() {
    if (m_dialog == NULL) {
        m_dialog = new Dialog*[2];
        m_dialog[0] = new Dialog(false);
        m_dialog[1] = new Dialog(true);
    }

    LoadIcons();
    CreateControls();
    return 0;
}

void MainWindow::LoadIcons() {
    m_iconSwitchToCrypto = (HICON)LoadImageW(
        NULL,
        L"./assets/crypto.ico",
        IMAGE_ICON,
        81, 27,
        LR_LOADFROMFILE
    );

    if (m_iconSwitchToCrypto == NULL) {
        MessageBoxW(NULL, L"Failed to load icons", L"Error", MB_OK);
    }

    m_cursorHand = LoadCursor(NULL, IDC_HAND);
}

void MainWindow::CreateControls() {
    m_hSwitchToCrypto = CreateWindowExW(
        0,
        L"button",
        L"",
        WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
        10, 10, 81, 27,
        Window(),
        (HMENU)IDC_SWITCH_BUTTON1,
        NULL,
        NULL
    );

    if (m_iconSwitchToCrypto) {
        SendMessageW(m_hSwitchToCrypto, BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)m_iconSwitchToCrypto);
    }
}

LRESULT MainWindow::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(Window(), &ps);

    HBRUSH hBrush = CreateSolidBrush(BG_COLOR);
    FillRect(hdc, &ps.rcPaint, hBrush);
    DeleteObject(hBrush);

    EndPaint(Window(), &ps);
    return 0;
}

LRESULT MainWindow::OnClose() {
    if (GetDialog(1)->ShowDialog(Window(), L"정말 종료하시겠습니까?") == IDOK) {
        DestroyWindow(Window());
    } 
    return 0;
}

LRESULT MainWindow::OnDestroy() {
    if (m_dialog != NULL) {
        delete m_dialog[0];
        delete m_dialog[1];
        delete[] m_dialog;
        m_dialog = NULL;
    }

    PostQuitMessage(m_nextWindowCode);
    return -1;
}

LRESULT MainWindow::OnSize(WPARAM wParam, LPARAM lParam) {
    int width = LOWORD(lParam);
    int height = HIWORD(lParam);
    OnSizeHandle(Window(), (UINT)wParam, width, height);

    AdjustControlPositions(width, height);
    return 0;
}

void MainWindow::AdjustControlPositions(int width, int height) {
    SetWindowPos(m_hSwitchToCrypto, NULL, 10, 10, 81, 27, SWP_NOZORDER);
}

void MainWindow::OnSizeHandle(HWND hwnd, UINT state, int width, int height) {
    if (width < 640 || height < 480) {
        return;
    }

    if (state == SIZE_RESTORED) {
        ShowWindow(hwnd, SW_RESTORE);
    }

    InvalidateRect(hwnd, NULL, TRUE); 
}

LRESULT MainWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
    switch (LOWORD(wParam)) {
        case IDC_SWITCH_BUTTON1:
            OnButtonSwitch(1);
            break;
        default:
            return DefWindowProc(Window(), WM_COMMAND, wParam, lParam);
    }
    return 0;
}

void MainWindow::OnButtonSwitch(int mode) {
    if (mode == 1) {
        m_nextWindowCode = 1;
        DestroyWindow(Window());
    }
}

LRESULT MainWindow::OnDrawItem(WPARAM wParam, LPARAM lParam) {
    if (wParam == IDC_SWITCH_BUTTON1 && m_iconSwitchToCrypto) {
        DrawButton(Window(), (LPDRAWITEMSTRUCT)lParam, m_iconSwitchToCrypto);
    }
    return 0;
}

void MainWindow::DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon) {
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
