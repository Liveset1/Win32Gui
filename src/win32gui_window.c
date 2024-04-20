#include "win32gui_window.h"
#include "win32gui_internal.h"

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
        Win32Window *window = (Win32Window *) userData;
        
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

    return 0;
}

// Initialization

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
        window->parentHandle, // Parent Handle
        NULL, // HMENU
        wndClassEx.hInstance, // HINSTANCE
        NULL // Additional Data
    );

    if (!window->handle)
        return 0;
    
    return 1;
}

Win32Window *initialize_window(WNDCLASSEX wndClassEx, Win32Size size, DWORD styles, HWND parentWindowHandle)
{
    // Register class
    RegisterClassEx(&wndClassEx);

    // Initialize window
    Win32Window *window = {0};
    window->parentHandle = parentWindowHandle;
    window->instance = wndClassEx.hInstance;
    window->size = size;
    window->styles = styles;

    if (create_window(window, wndClassEx)) {
        return window;
    } else {
        perror("initialize_window()");
        return NULL;
    }
}

// Configurations

void set_window_visibility(Win32Window *window, int visible)
{
    ShowWindow(window->handle, visible);
}

void set_window_title(Win32Window *window, const char *title)
{
    SetWindowText(window->handle, title);
    UpdateWindow(window->handle);
}

// Events

void window_poll_events() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}