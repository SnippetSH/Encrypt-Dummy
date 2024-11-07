#ifndef ENTRY_H
#define ENTRY_H

#ifdef __cplusplus
extern "C" {
#endif

// Windows API 함수는 내부적으로만 사용하므로 extern "C"에서 제외
#ifdef __cplusplus
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
#endif

// C에서 호출할 함수만 extern "C"로 선언
int startApp();

#ifdef __cplusplus
}
#endif

#endif
