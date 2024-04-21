#include "win32gui_window.h"
#include "win32gui_internal.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Win32AppData
{
    WNDCLASS wndClass;
    Win32Size size;
    DWORD styles;
} Win32AppData;

typedef struct Win32Window 
{
    HINSTANCE instance;
    HWND handle;
    HWND parentHandle;

    // App Data
    Win32AppData *appData;
} Win32Window;

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

Win32AppData *initialize_app_data(Win32Size size, DWORD styles) {
    Win32AppData *appData = malloc(sizeof(Win32AppData));
    if (!appData) {
        fprintf(stderr, "Error: Failed to allocate memory for Win32AppData\n");
        return NULL;
    }

    appData->size = size;
    appData->styles = styles;
    appData->wndClass = (WNDCLASS) { sizeof(WNDCLASS) };
    // WNDCLASS TYPE MEMEBERS
    appData->wndClass.lpfnWndProc = WndProc;
    appData->wndClass.hInstance = GetModuleHandle(NULL);
    // appData->wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    // appData->wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    appData->wndClass.lpszClassName = L"Win32Gui_Window";
    
    return appData;
}


int create_window(const wchar_t window_title[], Win32Window *window, Win32AppData *appData) {
    void *parentHandle = 0;

    if (window->parentHandle) {
        parentHandle = window->parentHandle;
    }

    window->handle = CreateWindow(
        appData->wndClass.lpszClassName, // Class Name
        window_title, // Title
        appData->styles, // Styles
        CW_USEDEFAULT, // X
        CW_USEDEFAULT, // Y
        appData->size.wid, // Width
        appData->size.hei, // Height
        0,
        parentHandle,
        appData->wndClass.hInstance,
        NULL
    );

    if (!window->handle) {
        printf("Failed to create handle\n");
        return 0;
    }
    
    return 1;
}

Win32Window *initialize_window(const wchar_t window_title[], Win32AppData *appData, HWND parentWindowHandle)
{
    if (!appData) {
        fprintf(stderr, "Error: AppData pointer is NULL!\n");
        return NULL;
    }
    // Register class
    if (!RegisterClass(&appData->wndClass)) {
        fprintf(stderr, "Error: Failed to register window class\n");
        free(appData);
        return NULL;
    }

    // Initialize window
    Win32Window *window = malloc(sizeof(Win32Window));
    window->appData = appData;
    if (!window) {
        fprintf(stderr, "Error: Failed to allocate memory for Win32Window\n");
        return NULL;
    }

    if (parentWindowHandle) {
        window->parentHandle = parentWindowHandle; // Set parent handle
    }

    if (create_window(window_title, window, appData)) {
        // Set user data for the window handle
        SetWindowLongPtr(window->handle, GWLP_USERDATA, (LONG_PTR)window);
        return window;
    } else {
        fprintf(stderr, "Error: Failed to create window\n");
        destroy_win32_window(window);
        return NULL;
    }
}

void destroy_win32_window(Win32Window *window)
{
    if (window) {
        if (window->appData) {
            free(window->appData);
        }
        free(window);
    }
}

void set_window_visibility(Win32Window *window, int visible)
{
    ShowWindow(window->handle, visible ? SW_SHOW : SW_HIDE);
}

void set_window_title(Win32Window *window, const wchar_t window_title[])
{
    SetWindowText(window->handle, window_title);
    UpdateWindow(window->handle);
}

void window_poll_events() {
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
