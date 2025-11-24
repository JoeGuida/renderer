#ifndef RENDERER_WIN32_INPUT_HPP
#define RENDERER_WIN32_INPUT_HPP

#include <array>
#include <optional>
#include <vector>

#include "input_codes.hpp"
#include "platform.hpp"

struct PlatformInput {
    bool initialized;
    std::array<RAWINPUTDEVICE, 2> devices;
    void setup_input_devices(HWND hwnd);
    std::optional<KeyEvent> get_key_event(LPARAM lparam);
    std::optional<std::vector<BYTE>> get_raw_input_data(LPARAM lparam);
};

#endif
