#ifndef GL_RENDERER_WIN32_WINDOW_HPP
#define GL_RENDERER_WIN32_WINDOW_HPP

#include <memory>

#include "platform.hpp"
#include  "renderer.hpp"

struct PlatformWindow {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
};

void run_message_loop(PlatformWindow* window, Renderer* renderer);
LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
std::expected<std::unique_ptr<PlatformWindow>, std::string> initialize_window(HINSTANCE instance, int show_window_flags,
                                                     int width, int height, 
                                                     const wchar_t* class_name, const wchar_t* window_title);

#endif
