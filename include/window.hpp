#ifndef RENDERER_WINDOW_HPP
#define RENDERER_WINDOW_HPP

#include <memory>

#if defined(_WIN64) || defined(_WIN32)
    #include <platform/win32_window.hpp>
#endif

#include "input.hpp"

struct PlatformWindow;

struct Window {
    std::unique_ptr<PlatformWindow> platform_window;
    std::unique_ptr<Input> input;
    int width;
    int height;
};

#endif

