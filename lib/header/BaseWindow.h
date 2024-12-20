#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <windows.h>
#include <stdio.h>
#include "Dialog.h"

#define BG_COLOR RGB(226, 231, 255)
#define TEXT_COLOR RGB(57, 57, 57)
#define EDIT_COLOR RGB(209, 209, 212)

class IBaseWindow {
public:
    virtual ~IBaseWindow() = default;
    virtual PCWSTR ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
    virtual BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = 520,
        int nHeight = 320,
        HWND hWndParent = NULL,
        HMENU hMenu = NULL
    ) = 0;

    virtual HWND Window() const = 0;
    virtual int GetNextWindowCode() = 0;
};

template <class T>
class BaseWindow : public IBaseWindow {
public:
    BaseWindow() : m_hwnd(NULL) {}
    HWND Window() const { return m_hwnd; }
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        T* pThis = NULL;

        if (msg == WM_NCCREATE) {
            CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = static_cast<T*>(cs->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

            pThis->m_hwnd = hwnd;
        } else {
            pThis = reinterpret_cast<T*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis) {
            return pThis->HandleMessage(msg, wParam, lParam);
        } else {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    BOOL Create(
        PCWSTR lpWindowName,
        DWORD dwStyle,
        DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int nWidth = 520,
        int nHeight = 320,
        HWND hWndParent = NULL,
        HMENU hMenu = NULL
    ) {
        WNDCLASSW wc = {};

        // 윈도우 클래스 설정
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = T::WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = ClassName();

        // cursor, icon, background, menu name
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hbrBackground = CreateSolidBrush(RGB(82,94,152));
        wc.lpszMenuName = NULL;

        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;

        // std::cout << "class name: " << ClassName() << "\n";
        if (!RegisterClassW(&wc)) {
            printf("RegisterClassW failed: %d\n", GetLastError());
            return FALSE;
        }

        m_hwnd = CreateWindowW(
            ClassName(),
            lpWindowName,
            dwStyle,
            x, y, nWidth, nHeight,
            hWndParent, hMenu,
            GetModuleHandle(NULL),
            this
        );  

        return (m_hwnd ? TRUE : FALSE);
    }

protected:
    virtual PCWSTR ClassName() const = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HWND m_hwnd;

    static Dialog** m_dialog;

public:
    static Dialog* GetDialog(int index);
};

#endif // GUI_H
