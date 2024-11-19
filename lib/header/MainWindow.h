#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BaseWindow.h"
#include "Dialog.h"
#include "../../resources/resource.h"

class MainWindow : public BaseWindow<MainWindow> {
// member variables
/* 
    Crypto: 파일 암호화 및 복호화
    Dummy: 빈 파일 생성

    m_hSwitchToCrypto: 암호화 버튼
    m_hSwitchToDummy: 추가 예정
    m_iconSwitchToCrypto: 암호화 아이콘
    m_iconSwitchToDummy: 추가 예정
*/
private:
    HWND m_hSwitchToCrypto;
    HWND m_hSwitchToDummy; // 추가 예정
    HICON m_iconSwitchToCrypto;
    HICON m_iconSwitchToDummy; // 추가 예정
    HCURSOR m_cursorHand;
    int m_nextWindowCode;
// member functions
private:
    void LoadIcons();
    void CreateControls();
    void OnSizeHandle(HWND hwnd, UINT state, int width, int height);
    void OnButtonSwitch(int mode);
    void AdjustControlPositions(int width, int height);
    void DrawButton(HWND hwnd, LPDRAWITEMSTRUCT lpDrawItem, HICON icon);

// message handlers
private:
    LRESULT OnCreate();
    LRESULT OnClose();
    LRESULT OnPaint();
    LRESULT OnSetCursor(WPARAM wParam, LPARAM lParam);
    LRESULT OnSize(WPARAM wParam, LPARAM lParam);
    LRESULT OnDestroy();
    LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
    LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
    LRESULT OnGetMinMaxInfo(LPARAM lParam);

// public functions
public:
    MainWindow();
    PCWSTR ClassName() const override;
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    static Dialog* GetDialog(int index);
    int GetNextWindowCode() override;
};

#endif // MAINWINDOW_H
