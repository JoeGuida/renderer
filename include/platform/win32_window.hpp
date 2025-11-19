#ifndef GL_RENDERER_WIN32_WINDOW_HPP
#define GL_RENDERER_WIN32_WINDOW_HPP

#include "platform.hpp"

struct PlatformWindow {
    HWND hwnd;
    HDC hdc;
    HGLRC hglrc;
};

#endif
