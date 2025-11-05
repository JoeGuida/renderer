#include "input.hpp"

#include "platform.hpp"

#include <hidsdi.h>
#include <SetupAPI.h>

#include <winuser.h>

#include <spdlog/spdlog.h>

#include "context.hpp"
#include "convert.hpp"

KeyCode get_keycode(ScanCode scancode) {
    return keybindings[scancode];
}

void remap(KeyCode keycode, ScanCode scancode) {
    keybindings[scancode] = keycode; 
}

void setup_input_devices(Input& input, HWND hwnd) {
    input.devices[0] = {
        .usUsagePage = 0x01, // HID_USAGE_PAGE_GENERIC
        .usUsage = 0x06,     // HID_USAGE_GENERIC_KEYBOARD
        .dwFlags = 0,
        .hwndTarget = hwnd
    };

    input.devices[1] = {
        .usUsagePage = 0x01, // HID_USAGE_PAGE_GENERIC
        .usUsage = 0x02,     // HID_USAGE_GENERIC_MOUSE
        .dwFlags = 0,
        .hwndTarget = hwnd
    };

    if(RegisterRawInputDevices(input.devices.data(), input.devices.size(), sizeof(RAWINPUTDEVICE)) == FALSE) {
        spdlog::error("RegisterRawInputDevices failed!"); 
    }
}

void handle_inputs(Input& input, LPARAM lparam, HWND hwnd, Context& context) {
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
        KeyCode keycode = get_keycode(scancode);
        KeyState state = KeyState::Undefined;

        if((raw->data.keyboard.Flags & RI_KEY_MAKE) == 0) {
            state = KeyState::Down;
        } 
        else if((raw->data.keyboard.Flags & RI_KEY_BREAK) == 0) {
           state = KeyState::Up; 
        }

        if(keycode != KeyCode::Undefined || state != KeyState::Undefined) {
            keypress(keycode, state, hwnd, context);
        }
    }

    delete[] lpb;
}

void keypress(KeyCode key, KeyState state, HWND hwnd, Context& context) {
    Input& input = *context.input;

    switch(key) {
        case KeyCode::Quit: {
            DestroyWindow(hwnd);
            break;
        }
        case KeyCode::Forward: {
            input.is_forward_pressed = (state == KeyState::Up) ? false : true;
            break; 
        }
        case KeyCode::Back: {
            input.is_back_pressed = (state == KeyState::Up) ? false : true;
            break; 
        }
        case KeyCode::Left: {
            input.is_left_pressed = (state == KeyState::Up) ? false : true;
            break; 
        }
        case KeyCode::Right: {
            input.is_right_pressed = (state == KeyState::Up) ? false : true;
            break; 
        }
        default: {
            break;
        }
    }
}
 
