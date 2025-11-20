#ifndef GL_RENDERER_WINDOW_HPP
#define GL_RENDERER_WINDOW_HPP

#include <memory>

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

#endif

