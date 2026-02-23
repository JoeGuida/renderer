#include <platform/windows/win32_window.hpp>

#include <functional>

std::pair<uint32_t, uint32_t> get_window_size(PlatformWindow* window) {
    RECT client_rect;
    GetClientRect(window->hwnd, &client_rect);
    uint32_t width = static_cast<uint32_t>(client_rect.right - client_rect.left);
    uint32_t height = static_cast<uint32_t>(client_rect.bottom - client_rect.top);

    return { width, height };
}

void run_window(PlatformWindow* handle, std::function<void()> draw) {
    MSG message;
    ZeroMemory(&message, sizeof(MSG));
    while (true) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        draw();
    }
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE: {
            return 0;
        }
        case WM_INPUT: {
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

std::expected<std::unique_ptr<PlatformWindow>, std::string> initialize_window(HINSTANCE instance, int show_window_flags, int width, int height, const wchar_t* class_name, const wchar_t* window_title) {
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
                                 NULL, NULL, instance, nullptr);

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

    return handle;
}
