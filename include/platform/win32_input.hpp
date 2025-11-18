#ifndef GL_RENDERER_WIN32_INPUT_HPP
#define GL_RENDERER_WIN32_INPUT_HPP

#include <functional>
#include <optional>

#include "platform.hpp"

struct Win32Input {
    std::array<RAWINPUTDEVICE, 2> devices;

    void setup_input_devices(HWND hwnd);
    ScanCode get_inputs(LPARAM lparam, HWND hwnd);
    ScanCode get_keyboard_input(RAWINPUT* raw);
};

#endif
