#ifndef GL_RENDERER_WINDOW_HPP
#define GL_RENDERER_WINDOW_HPP

#include <expected>
#include <memory>
#include <string>

#include "platform.hpp"

#include "renderer.hpp"

struct Window {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
    int width;
    int height;
};

void run_message_loop(HWND hwnd, HDC hdc, Renderer* renderer);
LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
std::expected<Window, std::string> initialize_window(HINSTANCE instance, int show_window_flags, 
                                                    int width, int height, 
                                                    const wchar_t* class_name, const wchar_t* window_title, Renderer* renderer);

#endif

