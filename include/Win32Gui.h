#ifndef WIN32GUI_H
#define WIN32GUI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

#include <windows.h>
#ifdef WIN32GUI_DLL
    #ifdef WIN32GUI_BUILD_DLL
        #define WIN32GUI_API __declspec(dllexport)
    #else
        #define WIN32GUI_API __declspec(dllimport)
    #endif
#endif

#ifndef WIN32GUI_API
    #define WIN32GUI_API
#endif

// =============================================================================
// Window
// =============================================================================

typedef struct Win32Window Win32Window;
typedef struct Win32Size {
    int wid, hei;
    int minWid, minHei;
    int maxWid, maxHei;
} Win32Size;

// Initialization

Win32Window *initialize_window(WNDCLASSEX wndClassEx, Win32Size size, DWORD styles, HWND parentWindowHandle);

// Configurations

void set_window_visibility(Win32Window *window, int visible);
void set_window_title(Win32Window *window, LPCWSTR title);

// Events

void window_poll_events();

// =============================================================================

#ifdef __cplusplus
}
#endif

#endif
#endif