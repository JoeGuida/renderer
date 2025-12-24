#ifndef WINDOW_WINDOW_HPP
#define WINDOW_WINDOW_HPP

#include <memory>

struct PlatformWindow;

struct Window {
    int width;
    int height;
    std::unique_ptr<PlatformWindow> handle;
};

#endif
