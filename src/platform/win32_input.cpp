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

    initialized = true;
}

std::optional<std::vector<BYTE>> PlatformInput::get_raw_input_data(LPARAM lparam) {
    UINT dw_size;
    if(GetRawInputData((HRAWINPUT)lparam, RID_INPUT, NULL, &dw_size, sizeof(RAWINPUTHEADER)) == -1) {
        spdlog::error("GetRawInputData failed");
        return std::nullopt;
    }

    std::vector<BYTE> buffer;
    buffer.resize(dw_size);
    if(GetRawInputData((HRAWINPUT)lparam, RID_INPUT, buffer.data(), &dw_size, sizeof(RAWINPUTHEADER)) != dw_size) {
        spdlog::error("GetRawInputData returning incorrect size");
        return std::nullopt;
    }

    return buffer;
}

std::optional<KeyEvent> PlatformInput::get_key_event(LPARAM lparam) {
    auto buffer = get_raw_input_data(lparam);
    if(!buffer.has_value()) {
        return std::nullopt;
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.value().data());
    if(raw->header.dwType == RIM_TYPEKEYBOARD) {
        RAWKEYBOARD& keyboard = raw->data.keyboard;

        ScanCode scancode = ScanCode::None;
        if(keyboard.Flags & RI_KEY_E0) { scancode = static_cast<ScanCode>(keyboard.MakeCode + 128); }
        else { scancode = static_cast<ScanCode>(keyboard.MakeCode); }

        KeyState state = KeyState::None;
        if (keyboard.Flags == RI_KEY_MAKE) { state = KeyState::Down; }
        if (keyboard.Flags == RI_KEY_BREAK) { state = KeyState::Up; }

        if(scancode == ScanCode::None && !(keyboard.Flags & RI_KEY_E0)) {
            spdlog::warn("could not determine scancode: {}", keyboard.MakeCode);
            return std::nullopt;
        }

        if(scancode == ScanCode::None && keyboard.Flags & RI_KEY_E0) {
            spdlog::warn("could not determine scancode: {}", keyboard.MakeCode + 128);
            return std::nullopt;
        }

        if(state == KeyState::None) {
            spdlog::warn("could not determine key state");
            return std::nullopt;
        }

        return KeyEvent {
            .scancode = scancode,
            .state = state
        };
    }

    return std::nullopt;
}

