#ifndef WIN32GUI_WINDOW_H
#define WIN32GUI_WINDOW_H

#include <stdbool.h>
#include <windows.h>

typedef struct Win32AppData Win32AppData;
typedef struct Win32Window Win32Window;
typedef struct {
    int wid, hei;
    int minWid, minHei;
    int maxWid, maxHei;
} Win32Size;

// Initialization

Win32AppData *initialize_app_data(const wchar_t class_name[], Win32Size size, DWORD styles);
Win32Window *initialize_window(const wchar_t window_title[], Win32AppData *appData, HWND parentWindowHandle);

// Deconstruction

void destroy_win32_window(Win32Window *window);

// Configurations

void set_window_visibility(Win32Window *window, int visible);
void set_window_title(Win32Window *window,  const char *window_title);

// Events

void window_poll_events();

#endif