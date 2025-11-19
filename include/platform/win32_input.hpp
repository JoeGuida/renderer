#ifndef GL_RENDERER_WIN32_INPUT_HPP
#define GL_RENDERER_WIN32_INPUT_HPP

#include <array>
#include <functional>
#include <optional>
#include <unordered_map>

#include "platform.hpp"

enum class ScanCode : USHORT {
    Esc          = 1,
    Backspace    = 14,
    Tab          = 15,
    Enter        = 28,
    LeftControl  = 29,
    RightControl = 29,
    LeftShift    = 42,
    RightShift   = 54,
    LeftAlt      = 56,
    RightAlt     = 56,
    Space        = 57,
    CapsLock     = 58,

    Minus    = 12,
    Equals   = 13,
    Divide   = 53,
    Decimal  = 83,
    Asterisk = 55,

    LeftSquareBracket  = 26,
    RightSquareBracket = 27,
    Semicolon          = 39,
    SingleQuote        = 40,
    BackTick           = 41,
    BackSlash          = 43,
    Comma              = 51,
    Period             = 52,

    Number1 = 2,
    Number2 = 3,
    Number3 = 4,
    Number4 = 5,
    Number5 = 6,
    Number6 = 7,
    Number7 = 8,
    Number8 = 9,
    Number9 = 10,
    Number0 = 11,

    A = 30,
    B = 48,
    C = 46,
    D = 32,
    E = 18,
    F = 33,
    G = 34,
    H = 35,
    I = 23,
    J = 36,
    K = 37,
    L = 38,
    M = 50,
    N = 49,
    O = 24,
    P = 25,
    Q = 16,
    R = 19,
    S = 31,
    T = 20,
    U = 22,
    V = 47,
    W = 17,
    X = 45,
    Y = 21,
    Z = 44,

    F1  = 59,
    F2  = 60,
    F3  = 61,
    F4  = 62,
    F5  = 63,
    F6  = 64,
    F7  = 65,
    F8  = 66,
    F9  = 67,
    F10 = 68,

    NumLock    = 69,
    ScrollLock = 70,

    Numpad7 = 71,
    Numpad8 = 72,
    Numpad9 = 73,
    Numpad4 = 75,
    Numpad5 = 76,
    Numpad6 = 77,
    Numpad1 = 79,
    Numpad2 = 80,
    Numpad3 = 81,
    Numpad0 = 82,

    NumpadPlus     = 78,
    NumpadEnter    = 28,
    NumpadDivide   = 53,
    NumpadMinus    = 74,
    NumpadMultiply = 55,

    Up     = 72,
    Left   = 75,
    Right  = 77,
    Down   = 80,
    Delete = 83,
};

enum class KeyCode : USHORT {
    Forward,
    Back,
    Left,
    Right,
    Quit,
};

enum class KeyState {
    Up,
    Down,
};

struct IInput {
    std::unordered_map<KeyCode, std::function<void()>> input_map;
    std::unordered_map<ScanCode, KeyCode> key_map;
    std::array<RAWINPUTDEVICE, 2> devices;

    void bind(KeyCode key, const std::function<void()>& function);
    std::optional<KeyCode> get_keycode(ScanCode scancode);
    void setup_input_devices(HWND hwnd);
    ScanCode get_inputs(LPARAM lparam, HWND hwnd);
    void keypress(KeyCode key, KeyState state);
};

#endif
