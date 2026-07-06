#ifndef INPUT_SCANCODE_HPP
#define INPUT_SCANCODE_CPP

#include <cstdint>
#include <string>

enum class ScanCode : uint32_t {
    A            = 0x1E,
    B            = 0x30,
    C            = 0x2E,
    D            = 0x20,
    E            = 0x12,
    F            = 0x21,
    G            = 0x22,
    H            = 0x23,
    I            = 0x17,
    J            = 0x24,
    K            = 0x25,
    L            = 0x26,
    M            = 0x32,
    N            = 0x31,
    O            = 0x18,
    P            = 0x19,
    Q            = 0x10,
    R            = 0x13,
    S            = 0x1F,
    T            = 0x14,
    U            = 0x16,
    V            = 0x2F,
    W            = 0x11,
    X            = 0x2D,
    Y            = 0x15,
    Z            = 0x2C,
    N0           = 0x0B,
    N1           = 0x02,
    N2           = 0x03,
    N3           = 0x04,
    N4           = 0x05,
    N5           = 0x06,
    N6           = 0x07,
    N7           = 0x08,
    N8           = 0x09,
    N9           = 0x0A,
    Tilde        = 0x29,
    Minus        = 0x0C,
    Equals       = 0x0D,
    BackSlash    = 0x2B,
    BackSpace    = 0x0E,
    Space        = 0x39,
    Tab          = 0x0F,
    Caps         = 0x3A,
    LeftShift    = 0x2A,
    Control      = 0x1D,
    Alt          = 0x38,
    RightShift   = 0x36,
    Enter        = 0x1C,
    Escape       = 0x01,
    F1           = 0x3B,
    F2           = 0x3C,
    F3           = 0x3D,
    F4           = 0x3E,
    F5           = 0x3F,
    F6           = 0x40,
    F7           = 0x41,
    F8           = 0x42,
    F9           = 0x43,
    F10          = 0x44,
    F11          = 0x57,
    F12          = 0x58,
    LeftBracket  = 0x1A,
    RightBracket = 0x1B,
    UpArrow      = 0x48, // E0
    LeftArrow    = 0x4B, // E0
    DownArrow    = 0x50, // E0
    RightArrow   = 0x4D, // E0
    SemiColon    = 0x27,
    Quote        = 0x28,
    Comma        = 0x33,
    Period       = 0x34,
    Slash        = 0x35,
    Undefined    = 0x00
};

std::string to_string(ScanCode scancode);

#endif

