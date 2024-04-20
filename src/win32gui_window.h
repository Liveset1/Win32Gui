#ifndef WIN32GUI_WINDOW_H
#define WIN32GUI_WINDOW_H

#include <stdbool.h>
#include <windows.h>

typedef struct Win32Window Win32Window;
typedef struct {
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

#endif