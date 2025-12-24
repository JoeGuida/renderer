#ifndef PLATFORM_WIN32_WINDOW_HPP
#define PLATFORM_WIN32_WINDOW_HPP

#include <expected>
#include <memory>
#include <string>

#include <platform/platform.hpp>

struct PlatformWindow {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
};

void run_window(PlatformWindow* handle);
LRESULT CALLBACK window_proc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
std::expected<std::unique_ptr<PlatformWindow>, std::string> initialize_window(HINSTANCE instance, int show_window_flags, int width, int height, const wchar_t* class_name, const wchar_t* window_title);

#endif
