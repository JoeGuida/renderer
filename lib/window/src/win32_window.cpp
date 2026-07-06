#include "window/win32_window.hpp"

#include <functional>

void run_window(PlatformWindow* handle, std::function<void()> input_callback) {
    MSG message;
    ZeroMemory(&message, sizeof(MSG));

    while(true) {
        while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                return;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        input_callback();

        SwapBuffers(handle->hdc);
    }
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE: {
            auto* cs = reinterpret_cast<CREATESTRUCT*>(lparam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
            return 0;
        }
        case WM_INPUT: {
            Input* input = reinterpret_cast<Input*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA)
            );

            if(input && input->initialized) {
                handle_inputs(*input, lparam);
            }

            return 0;
        }
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            RECT rect;
            GetClientRect(hwnd, &rect);

            HBRUSH brush = CreateSolidBrush(RGB(100, 149, 237)); // cornflower blue
            FillRect(hdc, &rect, brush);
            DeleteObject(brush);

            EndPaint(hwnd, &ps);

            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

std::expected<Window, std::string> initialize_window(HINSTANCE instance, int show_window_flags, uint32_t width, uint32_t height, const wchar_t* class_name, const wchar_t* window_title, Input* input) {
    WNDCLASSEX window_class {
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = window_proc,
        .cbClsExtra = NULL,
        .cbWndExtra = NULL,
        .hInstance = instance,
        .hIcon = LoadIcon(NULL, IDI_APPLICATION),
        .hCursor = LoadCursor(NULL, IDC_ARROW),
        .hbrBackground = nullptr,
        .lpszMenuName = NULL,
        .lpszClassName = class_name,
        .hIconSm = LoadIcon(NULL, IDI_APPLICATION)
    };

    if (!RegisterClassEx(&window_class)) {
        return std::unexpected("error registering window class");
    }

    HWND hwnd = CreateWindowEx(NULL, class_name, window_title, WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                               NULL, NULL, instance, &input);

    if (!hwnd) {
        return std::unexpected("error creating window! :: hwnd is null");
    }

    HDC hdc = GetDC(hwnd);
    if(!hdc) {
        return std::unexpected("error creating window! :: hdc is null");
    }

    ShowWindow(hwnd, show_window_flags);
    UpdateWindow(hwnd);

    std::unique_ptr<PlatformWindow> handle = std::make_unique<PlatformWindow>();
    handle->hwnd = hwnd;
    handle->hdc = hdc;

    return Window {
        .width = width,
        .height = height,
        .handle = std::move(handle)
    };
}

