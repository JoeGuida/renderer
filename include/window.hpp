#ifndef GL_RENDERER_WINDOW_HPP
#define GL_RENDERER_WINDOW_HPP

#include <expected>
#include <memory>
#include <string>

#include "renderer.hpp"

#if defined(_WIN64) || defined(_WIN32)
    #include <platform/win32_window.hpp>
#elif defined(__linux__)
    #include <platform/linux_window.hpp>
#endif

struct PlatformWindow;

struct Window {
    std::unique_ptr<PlatformWindow> platform_window;
    int width;
    int height;
};

std::expected<Window, std::string> initialize_window(HINSTANCE instance, int show_window_flags, 
                                                    int width, int height, 
                                                    const wchar_t* class_name, const wchar_t* window_title);

#endif

