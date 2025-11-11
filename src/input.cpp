#include "input.hpp"

#include <spdlog/spdlog.h>

void Input::bind(KeyCode key, const std::function<void()>& function) {
    input_map[key] = function;
}

std::optional<KeyCode> Input::get_keycode(ScanCode scancode) {
    if(key_bindings.contains(scancode)) {
        return key_bindings[scancode];
    }

    return std::nullopt;
}

void Input::setup_input_devices(HWND hwnd) {
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

void Input::handle_inputs(LPARAM lparam, HWND hwnd, Renderer& renderer) {
    UINT dw_size;
    GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER));
    LPBYTE lpb = new BYTE[dw_size];

    if(GetRawInputData((HRAWINPUT)lparam, RID_INPUT, lpb, &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
        spdlog::error("GetRawInputData returning incorrect size");
        return;
    }

    RAWINPUT* raw = (RAWINPUT*)lpb;

    if(!raw) {
        spdlog::error("RAWINPUT* raw is null");
        return;
    }

    if(raw->header.dwType == RIM_TYPEKEYBOARD) {
        ScanCode scancode = static_cast<ScanCode>(raw->data.keyboard.MakeCode);
        std::optional<KeyCode> keycode = get_keycode(scancode);
        KeyState state;

        if(raw->data.keyboard.Flags == RI_KEY_MAKE) {
            state = KeyState::Down;
        } 
        else if(raw->data.keyboard.Flags == RI_KEY_BREAK) {
            state = KeyState::Up;
        }

        if(keycode.has_value()) {
            keypress(keycode.value(), state);
        }
    }

    delete[] lpb;
}

void Input::keypress(KeyCode key, KeyState state) {
    key_states[key] = state;
}

bool Input::is_key_down(KeyCode key) {
    return key_states[key] == KeyState::Down;
}

