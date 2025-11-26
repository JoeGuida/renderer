#include <platform/win32_window.hpp>

#include <spdlog/spdlog.h>

#include "gl_loader.hpp"

void run_message_loop(PlatformWindow* window, Input* input, Renderer* renderer) {
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
            for(const auto& [action, bindings] : input->bindings) {
                for(const auto& key : input->keys[action]) {
                    for(const auto& binding : bindings) {
                        void* i = input;
                        if(input->input_states[key] == binding.state) {
                            binding.callback();
                        }
                    }
                }
            }

            RECT client_rect;
            GetClientRect(window->hwnd, &client_rect);
            int client_width = client_rect.right - client_rect.left;
            int client_height = client_rect.bottom - client_rect.top;

            bind_vao(renderer->vao);
            draw(*renderer, client_width, client_height); 
            SwapBuffers(window->hdc);
        }
    }
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE: {
            spdlog::info("WM_CREATE");
            LPCREATESTRUCT p_create_struct = reinterpret_cast<LPCREATESTRUCT>(lparam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_create_struct->lpCreateParams));

            Input* input = reinterpret_cast<Input*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            void* i = input;
            if(input && !input->platform_input->initialized) {
                input->platform_input->setup_input_devices(hwnd);
                input->platform_input->initialized = true;
            }
            return 0;
        }
        case WM_INPUT: {
            Input* input = reinterpret_cast<Input*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

            if(input) {
                std::optional<KeyEvent> key_event = input->platform_input->get_key_event(lparam);
                if(key_event.has_value()) {
                    KeyEvent& value = key_event.value();
                    input->input_states[value.scancode] = get_input_state(value.state, input->input_states[value.scancode]);
                }
            }
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

std::expected<std::unique_ptr<PlatformWindow>, std::string> initialize_window(HINSTANCE instance, int show_window_flags,
                                                     int width, int height, 
                                                     const wchar_t* class_name, const wchar_t* window_title, Input* input)
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
                                 NULL, NULL, instance, input);

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

    spdlog::info("window created successfully");
    return platform_window;
}

void PlatformWindow::close() {
    DestroyWindow(hwnd);
}

