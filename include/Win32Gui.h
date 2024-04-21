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

typedef struct Win32AppData Win32AppData;
typedef struct Win32Window Win32Window;
typedef struct {
    int wid, hei;
    int minWid, minHei;
    int maxWid, maxHei;
} Win32Size;

// Initialization

Win32AppData *initialize_app_data(Win32Size size, DWORD styles);
Win32Window *initialize_window(const wchar_t window_title[], Win32AppData *appData, HWND parentWindowHandle);

// Deconstruction

void destroy_win32_window(Win32Window *window);

// Configurations

void set_window_visibility(Win32Window *window, int visible);
void set_window_title(Win32Window *window,  const wchar_t window_title[]);

// Events

void window_poll_events();

// =============================================================================

#ifdef __cplusplus
}
#endif

#endif
#endif