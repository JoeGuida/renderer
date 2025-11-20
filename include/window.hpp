#ifndef RENDERER_WINDOW_HPP
#define RENDERER_WINDOW_HPP

#if defined(_WIN64) || defined(_WIN32)
    #include <platform/win32_window.hpp>
#elif defined(__linux__)
    #include <platform/linux_window.hpp>
#endif

#include <memory>

#include "input.hpp"

struct PlatformWindow;

struct Window {
    std::unique_ptr<PlatformWindow> platform_window;
    std::unique_ptr<Input> input;
    int width;
    int height;
};

#endif

