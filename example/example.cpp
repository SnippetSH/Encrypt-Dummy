#include <windows.h>

/*
    typedef struct tagWNDCLASS {
        UINT        style;            // 윈도우 스타일 지정 : 어떤 형태를 가질 것인가를 지정
        WNDPROC     lpfnWndProc;      // 메시지 핸들러 함수 지정 : 메시지가 발생할 때마다 지정한 함수가 호출
        int         cbClsExtra;       // 일종의 예약 영역 : Windows 운영체제 내부적으로 특수한 목적으로 사용되는 여분의 공간
        int         cbWndExtra;       // 일종의 예약 영역 : Windows 운영체제 내부적으로 특수한 목적으로 사용되는 여분의 공간
        HINSTANCE   hInstance;        // 윈도우 클래스를 사용하는 프로그램의 번호
        HICON       hIcon;            // 윈도우가 사용할 마우스 커서와 최소화 되었을 경우 출력될 아이콘
        HCURSOR     hCursor;          // LoadCursor 함수와 LoadIcon 함수를 사용하여 지정
        HBRUSH      hbrBackground;    // 윈도우의 배경색 지정
        LPCWSTR     lpszMenuName;     // 프로그램이 사용할 메뉴 지정
        LPCWSTR     lpszClassName;    // 윈도우 클래스의 이름 정의 : CreateWindow 함수에 전달됨
    } WNDCLASS;

    HWND CreateWindow(
        lpszClassName,     // 생성하고자 하는 윈도우의 클래스를 지정하는 문자열 (WndClass 구조체의 lpszClassName 멤버의 이름을 기입)
        lpszWindowName,    // 윈도우의 타이블바에 나타날 문자열
        dwStyle,           // 만들고자 하는 윈도우의 형태 지정
        x,                 // 윈도우 x 좌표
        y,                 // 윈도우 y 좌표
        nWidth,            // 윈도우 가로 너비
        nHeight,           // 윈도우 세로 너비
        hWndParent,        // 부모 윈도우가 있을 경우 부모 윈도우의 핸들을 지정
        hMenu,             // 윈도우에서 사용할 메뉴의 핸들
        hInstance,         // 윈도우를 만드는 주체, 프로그램의 핸들을 지정 (WinMain의 매개변수인 hInstance 대입)
        lpParam            // CREATESTRUCT라는 구조체의 번지로 특수한 목적에 사용 (보통 NULL)
    )
*/

/*
    typedef struct StateStruct {
    }
    와 같이 선언된 구조체는 윈도우 클래스의 추가 정보를 저장하는 데 사용됨.
*/

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void OnSize(HWND, UINT, int, int);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {
    HWND hwnd;
    WNDCLASSW WndClass;
    LPCWSTR mainClassName = L"MainWindow";

    // default settings for window class
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;

    // other settings for window class
    WndClass.lpszClassName = mainClassName;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    RegisterClassW(&WndClass);

    // StateStruct* state = new (std::nothrow) StateStruct;
    // 위처럼 구조체를 선언하여 추가 정보를 기록할 수 있음.

    hwnd = CreateWindowW(
        mainClassName,
        L"Test Window",
        WS_OVERLAPPEDWINDOW,
        150,
        150,
        200,
        200,
        NULL,
        NULL,
        hInstance,
        NULL // 추가 정보를 저장하는 데 사용되는 포인터
    );

    if (!hwnd) return 0; 

    ShowWindow(hwnd, nCmdShow); 
    
    MSG msg = { }; // 여기서 메시지란, gui와 유저의 상호작용으로 발생하는 모든 이벤트를 의미함.
    while (GetMessage(&msg, hwnd, 0, 0) > 0) {
        TranslateMessage(&msg); // 키보드 입력을 문자로 변환
        DispatchMessage(&msg); // 메시지를 처리하는 함수에 전달
    }

    return 0;
}

/*inline StateStruct* GetState(HWND hwnd) {
    return reinterpret_cast<StateStruct*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
}*/

LRESULT CALLBACK WndProc(HWND hwnd /*창에 대한 핸들*/, UINT msg /*메시지*/, WPARAM wParam /*추가 정보*/, LPARAM lParam /*추가 정보*/) {
    /*StateStruct* state;
    if (msg == WM_NCCREATE || msg == WM_CREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        state = static_cast<StateStruct*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(state));
    } else {
        state = GetState(hwnd);
    } */
    switch (msg) {
        case WM_CLOSE:
            if (MessageBoxW(hwnd, L"정말 종료하시겠습니까?", L"종료", MB_YESNO) == IDYES) {
                DestroyWindow(hwnd);
            }
            return 0;
        case WM_PAINT: // 윈도우 창의 크기가 변화하거나, 가려졌다가 보이거나 하는 등의 모든 변화가 생길 때 호출
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
            
                    
                FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
                EndPaint(hwnd, &ps);
            }
            return 0;
        case WM_SIZE:
            {
                int w = LOWORD(lParam);
                int h = HIWORD(lParam);

                OnSize(hwnd, (UINT)wParam, w, h);
            }

            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
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
