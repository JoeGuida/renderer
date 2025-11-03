#include "input.hpp"

#include <Windows.h>
#include <hidsdi.h>
#include <SetupAPI.h>

#include <winuser.h>

#include <spdlog/spdlog.h>

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

void handle_inputs(Input& input, LPARAM lparam, HWND hwnd) {
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

    TCHAR szTempOutput[256];
    if(raw->header.dwType == RIM_TYPEKEYBOARD) {
        if((raw->data.keyboard.Flags & RI_KEY_BREAK) == 0) {
            ScanCode scancode = static_cast<ScanCode>(raw->data.keyboard.MakeCode);
            KeyCode keycode = get_keycode(scancode);
            switch(keycode) {
                case KeyCode::Quit: {
                    DestroyWindow(hwnd);
                    break;
                }
                case KeyCode::Forward: {
                   break; 
                }
                case KeyCode::Back: {
                   break; 
                }
                case KeyCode::Left: {
                   break; 
                }
                case KeyCode::Right: {
                   break; 
                }
                default: {
                    break;
                }
            }
        }
    }

    if(raw->header.dwType == RIM_TYPEMOUSE) {
        if(raw->data.mouse.usFlags == MOUSE_MOVE_RELATIVE) {
            // do something with mouse movement
        }
    }

    delete[] lpb;
}
 
