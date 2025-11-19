#include "window.hpp"

#include <expected>
#include <string>

#include "platform.hpp"

#include <spdlog/spdlog.h>

#include "gl_loader.hpp"
#include "input.hpp"
#include "renderer.hpp"

void run_message_loop(PlatformWindow* window, Renderer* renderer) {
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
        else {
            RECT client_rect;
            GetClientRect(window->hwnd, &client_rect);

            int client_width = client_rect.right - client_rect.left;
            int client_height = client_rect.bottom - client_rect.top;

            glViewport(0, 0, client_width, client_height);
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(renderer->vao);

            draw(*renderer); 

            SwapBuffers(window->hdc);
        }
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

std::expected<Window, std::string> initialize_window(HINSTANCE instance, int show_window_flags, 
                                                     int width, int height, 
                                                     const wchar_t* class_name, const wchar_t* window_title) 
{
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
        return std::unexpected("error creating window");
    }

    HDC hdc = GetDC(hwnd);
    PIXELFORMATDESCRIPTOR pfd = {
        .nSize = sizeof(PIXELFORMATDESCRIPTOR),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = 32,
        .cDepthBits = 24,
        .cStencilBits = 8,
        .iLayerType = PFD_MAIN_PLANE
    };

    int pixel_format = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixel_format, &pfd);
    HGLRC temp_context = wglCreateContext(hdc);
    wglMakeCurrent(hdc, temp_context);

    load_gl_functions();

    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
    if(hglrc) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(temp_context);
        wglMakeCurrent(hdc, hglrc); 
    }

    ShowWindow(hwnd, show_window_flags);
    UpdateWindow(hwnd);

    std::unique_ptr<PlatformWindow> platform_window = std::make_unique<PlatformWindow>();
    platform_window->hwnd = hwnd;
    platform_window->hdc = hdc;
    platform_window->hglrc = hglrc;

    Window window;
    window.platform_window = std::move(platform_window);
    window.width = width;
    window.height = height;

    return window;
}
