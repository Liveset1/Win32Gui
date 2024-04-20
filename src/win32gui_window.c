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

int create_window(Win32Window *window, WNDCLASS wndClass) {
    // NOTE: ADD A WAY FOR USERS TO ADD THEIR OWN WNDPROC (NOT RECOMMENDED FROM PAST ME)
    wndClass.lpfnWndProc = WndProc;

    void *parentHandle = NULL;

    if (window->parentHandle) {
        parentHandle = window->parentHandle;
    }

    window->handle = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW, // Styles
        wndClass.lpszClassName, // Class Name
        wndClass.lpszClassName, // Window Title
        window->styles, // Styles
        CW_USEDEFAULT, // X
        CW_USEDEFAULT, // Y
        window->size.wid, // Width
        window->size.hei, // Height
        NULL, // Parent Handle
        NULL, // HMENU
        wndClass.hInstance, // HINSTANCE
        NULL // Additional Data
    );

    if (!window->handle) {
        printf("Failed to create handle\n");
        return 0;
    }
    
    return 1;
}

WNDCLASS new_window_props(const char *window_title) {
    WNDCLASS wc = { sizeof(wc) };

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = window_title;

    return wc;
}

Win32Window *initialize_window(WNDCLASS wndClass, Win32Size size, DWORD styles, HWND parentWindowHandle)
{
    // Register class
    if (!RegisterClass(&wndClass)) {
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
    window->instance = wndClass.hInstance;
    window->size = size;
    window->styles = styles;
    if (parentWindowHandle) {
        window->parentHandle = parentWindowHandle; // Set parent handle
    }

    if (create_window(window, wndClass)) {
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
