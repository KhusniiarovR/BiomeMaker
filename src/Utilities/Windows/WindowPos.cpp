#include "WindowPos.h"
#if defined(_WIN32)

    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    extern "C" void *GetWindowHandle(void);

    void SetWindowPosition(int x, int y)
    {
        HWND hwnd = static_cast<HWND>(GetWindowHandle());
        if (hwnd)
            SetWindowPos(hwnd, nullptr, x, y, 0, 0,
                         SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
    }

#elif defined(__linux__) || defined(__APPLE__)
    void SetWindowPosition(int, int) {}
#endif