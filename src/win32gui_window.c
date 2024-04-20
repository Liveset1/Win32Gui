#include "win32gui_window.h"
#include "win32gui_internal.h"

#include <stdio.h>
#include <stdlib.h>

struct Win32Window 
{
    HINSTANCE instance;
    HWND handle;
    HWND parentHandle;

    // App Data
    Win32Size size;
    DWORD styles;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // Get Window Long Pointer
    LONG_PTR userData = GetWindowLongPtr(hWnd, GWLP_USERDATA);
    if (userData != 0) {
        Win32Window *window = (Win32Window *)userData;

        switch (message) {
            case WM_DESTROY: {
                PostQuitMessage(0);
                break;
            }
            default: {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}

int create_window(Win32Window *window, WNDCLASSEX wndClassEx) {
    // NOTE: ADD A WAY FOR USERS TO ADD THEIR OWN WNDPROC (NOT RECOMMENDED FROM PAST ME)
    wndClassEx.lpfnWndProc = WndProc;
    window->handle = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW, // Styles
        wndClassEx.lpszClassName, // Class Name
        wndClassEx.lpszClassName, // Window Title
        window->styles, // Styles
        CW_USEDEFAULT, // X
        CW_USEDEFAULT, // Y
        window->size.wid, // Width
        window->size.hei, // Height
        window->parentHandle || NULL, // Parent Handle
        NULL, // HMENU
        wndClassEx.hInstance, // HINSTANCE
        NULL // Additional Data
    );

    if (!window->handle) {
        printf("Failed to create handle\n");
        return 0;
    }
    
    return 1;
}

Win32Window *initialize_window(WNDCLASSEX wndClassEx, Win32Size size, DWORD styles, HWND parentWindowHandle)
{
    // Register class
    if (!RegisterClassEx(&wndClassEx)) {
        fprintf(stderr, "Error: Failed to register window class\n");
        return NULL;
    }

    // Initialize window
    Win32Window *window = malloc(sizeof(Win32Window));
    if (!window) {
        fprintf(stderr, "Error: Failed to allocate memory for Win32Window\n");
        return NULL;
    }

    // Initialize Win32Window struct members
    window->instance = wndClassEx.hInstance;
    window->size = size;
    window->styles = styles;
    if (parentWindowHandle) {
        window->parentHandle = parentWindowHandle; // Set parent handle
    }

    if (create_window(window, wndClassEx)) {
        // Set user data for the window handle
        SetWindowLongPtr(window->handle, GWLP_USERDATA, (LONG_PTR)window);
        return window;
    } else {
        fprintf(stderr, "Error: Failed to create window\n");
        free(window); // Free allocated memory
        return NULL;
    }
}

void set_window_visibility(Win32Window *window, int visible)
{
    ShowWindow(window->handle, visible ? SW_SHOW : SW_HIDE);
}

void set_window_title(Win32Window *window, const char *title)
{
    SetWindowText(window->handle, title);
    UpdateWindow(window->handle);
}

void window_poll_events() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
