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
    HWND m_hStaticUserKeyLength;
    HWND m_hButtonOpen;
    HWND m_hButtonEncrypt;
    HWND m_hButtonDecrypt;
    HWND m_hStaticFileName;
    HICON m_iconOpen;
    HICON m_iconEncrypt;
    HICON m_iconDecrypt;
    HCURSOR m_cursorHand;

    void CreateControls();
    void LoadIcons();
    void AdjustControlPositions(int width, int height);
    wchar_t* makeFileName(wchar_t* fileName);

    // 메시지 핸들러 함수
    LRESULT OnCreate();
    LRESULT OnClose();
    LRESULT OnPaint();
    LRESULT OnSize(WPARAM wParam, LPARAM lParam);
    LRESULT OnDestroy();
    LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
    LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
    LRESULT OnSetCursor(WPARAM wParam, LPARAM lParam);
    LRESULT OnCtlColorStatic(WPARAM wParam, LPARAM lParam);
    LRESULT OnCtlColorEdit(WPARAM wParam, LPARAM lParam);
    LRESULT OnGetMinMaxInfo(LPARAM lParam);
    void OnButtonRun(HWND hwnd, LPARAM lParam, bool isEncrypt);
    void OnButtonOpen(HWND hwnd, LPARAM lParam);
    void DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon);
    void OnSizeHandle(HWND hwnd, UINT state, int width, int height);

public:
    MainWindow();
    PCWSTR ClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    static Dialog* GetDialog(int index);
};

#endif
