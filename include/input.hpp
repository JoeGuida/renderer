#ifndef GL_RENDERER_INPUT_HPP
#define GL_RENDERER_INPUT_HPP

#include <array>
#include <span>
#include <string>
#include <unordered_map>

#include "platform.hpp"

#include <glm/vec3.hpp>

class Context;

enum class ScanCode : USHORT {
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
    Escape = 1,
    Undefined = 0
};

enum class KeyCode : USHORT {
    Forward,
    Back,
    Left,
    Right,
    Quit,
    Undefined
};

enum class KeyState {
    Up,
    Down,
    Undefined
};

static std::unordered_map<ScanCode, KeyCode> keybindings {
    { ScanCode::W, KeyCode::Forward },
    { ScanCode::S, KeyCode::Back },
    { ScanCode::A, KeyCode::Left },
    { ScanCode::D, KeyCode::Right },
    { ScanCode::Escape, KeyCode::Quit },
    { ScanCode::Undefined, KeyCode::Undefined }
};

struct Input {
    std::array<RAWINPUTDEVICE, 2> devices;
    bool initialized;
    glm::vec3 mouse_position;

    bool is_forward_pressed = false;
    bool is_back_pressed = false;
    bool is_left_pressed = false;
    bool is_right_pressed = false;
};

KeyCode get_keycode(ScanCode scancode);
void remap(KeyCode keycode, ScanCode scancode);
void setup_input_devices(Input& input, HWND hwnd);
void handle_inputs(Input& input, LPARAM lparam, HWND hwnd, Context& context);
void keypress(KeyCode key, KeyState state, HWND hwnd, Input& input);
glm::vec3 get_direction(const Input& input);

#endif
