#include <platform/win32_input.hpp>

#include <spdlog/spdlog.h>

void PlatformInput::setup_input_devices(HWND hwnd) {
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

std::optional<KeyEvent> get_key_event(LPARAM lparam, HWND hwnd) {
    UINT dw_size;
    if(GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER)) == -1) {
        spdlog::error("GetRawInputData failed");
        return std::nullopt;
    }

    std::vector<BYTE> buffer;
    buffer.reserve(dw_size);
    if(GetRawInputData((HRAWINPUT)lparam, RID_INPUT, buffer.data(), &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
        spdlog::error("GetRawInputData returning incorrect size");
        return std::nullopt;
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data());
    if(!raw) {
        spdlog::error("RAWINPUT* raw is null");
        return std::nullopt;
    }

    if(raw->header.dwType == RIM_TYPEKEYBOARD) {
        ScanCode scancode;
        if(raw->data.keyboard.Flags | RI_KEY_E0) {
            scancode = static_cast<ScanCode>(raw->data.keyboard.MakeCode + 128);
        }
        else {
            scancode = static_cast<ScanCode>(raw->data.keyboard.MakeCode);
        }

        KeyState state;
        if(raw->data.keyboard.Flags | RI_KEY_MAKE) {
            state = KeyState::Down;
        } 
        else if(raw->data.keyboard.Flags == RI_KEY_BREAK) {
            state = KeyState::Up; 
        }

        return KeyEvent {
            .scancode = scancode,
            .state = state
        };
    }

    return std::nullopt;
}
