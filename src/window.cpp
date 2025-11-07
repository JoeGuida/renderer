#include "window.hpp"

#include <expected>
#include <string>

#include "platform.hpp"

#include <spdlog/spdlog.h>

#include "context.hpp"
#include "gl_loader.hpp"
#include "renderer.hpp"

void run_message_loop(HWND hwnd, HDC hdc, Context& context) {
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
            GetClientRect(hwnd, &client_rect);

            int client_width = client_rect.right - client_rect.left;
            int client_height = client_rect.bottom - client_rect.top;

            glViewport(0, 0, client_width, client_height);
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(context.renderer->vao);

            glm::vec4 direction(0.0f);
            float velocity = 0.01f;
            if(context.input->is_forward_pressed) {
                direction += glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * velocity;
            }
            if(context.input->is_back_pressed) {
                direction += glm::vec4(0.0f, -1.0f, 0.0f, 0.0f) * velocity;
            }
            if(context.input->is_left_pressed) {
                direction += glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f) * velocity;
            }
            if(context.input->is_right_pressed) {
                direction += glm::vec4(1.0f, 0.0f, 0.0f, 0.0f) * velocity;
            }

            update_positions(*context.renderer, 0, direction);
            draw(*context.renderer); 

            SwapBuffers(hdc);
        }
    }
}

LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
        case WM_CREATE: {
            LPCREATESTRUCT p_create_struct = reinterpret_cast<LPCREATESTRUCT>(lparam);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(p_create_struct->lpCreateParams));
            Context* context = reinterpret_cast<Context*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if(context->input && !context->input->initialized) {
                setup_input_devices(*context->input, hwnd);
                context->input->initialized = true;
            }

            return 0;
        }
        case WM_INPUT: {
            Context* context = reinterpret_cast<Context*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if(context->input && context->input->initialized) {
                handle_inputs(*context->input, lparam, hwnd, *context);
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

std::expected<Window, std::string> initialize_window(HINSTANCE instance, int show_window_flags, 
                                                     int width, int height, 
                                                     const wchar_t* class_name, const wchar_t* window_title, Context& context) 
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
                                 NULL, NULL, instance, &context);

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

    return Window {
        .hwnd = hwnd,
        .hdc = hdc,
        .hglrc = hglrc,
        .width = width,
        .height = height
    };
}
