#include "platform/win32_input.hpp"

#include <vector>

#include <spdlog/spdlog.h>

void Win32Input::setup_input_devices(HWND hwnd) {
    devices[0] = {
        .usUsagePage = 0x01, // HID_USAGE_PAGE_GENERIC
        .usUsage = 0x06,     // HID_USAGE_GENERIC_KEYBOARD
        .dwFlags = 0,
        .hwndTarget = hwnd
    };

    devices[1] = {
        .usUsagePage = 0x01, // HID_USAGE_PAGE_GENERIC
        .usUsage = 0x02,     // HID_USAGE_GENERIC_MOUSE
        .dwFlags = 0,
        .hwndTarget = hwnd
    };

    if(RegisterRawInputDevices(devices.data(), devices.size(), sizeof(RAWINPUTDEVICE)) == FALSE) {
        spdlog::error("RegisterRawInputDevices failed!"); 
    }
}

ScanCode Win32Input::get_inputs(LPARAM lparam, HWND hwnd, Renderer& renderer) {
    UINT dw_size;
    if(GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER)) == -1) {
        spdlog::error("GetRawInputData failed");
        return;
    }

    std::vector<BYTE> bytes;
    if(GetRawInputData((HRAWINPUT)lparam, RID_INPUT, bytes.data(), &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
        spdlog::error("GetRawInputData returning incorrect size");
        return;
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(bytes.data());

    if(!raw) {
        spdlog::error("RAWINPUT* raw is null");
        return;
    }

    if(raw->header.dwType == RIM_TYPEKEYBOARD) {
        return get_keyboard_input(raw);
    }

    if(raw->header.dwType == RIM_TYPEMOUSE) {
        //return get_mouse_input(raw);
    }
}

ScanCode Win32Input::get_keyboard_input(RAWINPUT* raw) {
        ScanCode scancode = static_cast<ScanCode>(raw->data.keyboard.MakeCode);
        KeyState state;

        if(raw->data.keyboard.Flags == RI_KEY_MAKE) {
            state = KeyState::Down;
        }
        else if(raw->data.keyboard.Flags == RI_KEY_BREAK) {
            state = KeyState::Up;
        }

        if(keycode.has_value()) {
            return scancode;
        }
}

