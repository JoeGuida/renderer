#ifndef GL_RENDERER_INPUT_HPP
#define GL_RENDERER_INPUT_HPP

#if defined(_WIN64) || defined(_WIN32)
    #include <platform/win32_input.hpp>
#elif defined(__linux__)
    #include <platform/linux_input.hpp>
#endif

#include <cstdint>
#include <functional>
#include <queue>
#include <unordered_map>


enum class ScanCode : uint32_t {
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

    Number0 = 11,
    Number1 = 2,
    Number2 = 3,
    Number3 = 4,
    Number4 = 5,
    Number5 = 6,
    Number6 = 7,
    Number7 = 8,
    Number8 = 9,
    Number9 = 10,

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

    Numpad0 = 82,
    Numpad1 = 79,
    Numpad2 = 80,
    Numpad3 = 81,
    Numpad4 = 75,
    Numpad5 = 76,
    Numpad6 = 77,
    Numpad7 = 71,
    Numpad8 = 72,
    Numpad9 = 73,

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

enum class InputAction : uint32_t {
    MoveForward,
    MoveBack,
    MoveLeft,
    MoveRight,
    Quit
};

enum class InputState : uint32_t {
    Down,
    Up,
    Pressed,
    Released
};

struct InputBinding {
    InputAction action;
    std::function<void()> callback;
    bool repeat;
};

inline std::unordered_map<ScanCode, InputAction> action_to_scancode {
    { InputAction::MoveForward, ScanCode::W },
    { InputAction::MoveLeft, ScanCode::A },
    { InputAction::MoveBack, ScanCode::S },
    { InputAction::MoveRight, ScanCode::D },
    { InputAction::Quit, ScanCode::Esc }
};

inline std::unordered_map<InputAction, InputState> action_to_state;
inline std::unordered_map<InputAction, InputState> previous_action_to_state;

struct Input {
    bool is_key_down(const KeyCode& key);
    bool is_key_up(const KeyCode& key);
    bool is_key_pressed(const KeyCode& key);
    bool is_key_released(const KeyCode& key);

    void bind(const InputBinding& binding);

    std::queue<InputBinding> bindings;
};

#endif
