#ifndef GL_RENDERER_INPUT_HPP
#define GL_RENDERER_INPUT_HPP

#include <array>
#include <functional>
#include <optional>
#include <unordered_map>

#include "platform.hpp"
#include "renderer.hpp"

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

static std::unordered_map<ScanCode, KeyCode> key_bindings {
    { ScanCode::W, KeyCode::Forward },
    { ScanCode::S, KeyCode::Back },
    { ScanCode::A, KeyCode::Left },
    { ScanCode::D, KeyCode::Right },
    { ScanCode::Escape, KeyCode::Quit }
};

static std::unordered_map<KeyCode, KeyState> key_states {
    { KeyCode::Forward, KeyState::Up },
    { KeyCode::Back, KeyState::Up },
    { KeyCode::Left, KeyState::Up },
    { KeyCode::Right, KeyState::Up },
    { KeyCode::Quit, KeyState::Up }
};

struct Input {
    std::unordered_map<KeyCode, std::function<void()>> input_map;
    std::unordered_map<ScanCode, KeyCode> key_map;
    std::array<RAWINPUTDEVICE, 2> devices;

    void bind(KeyCode key, const std::function<void()>& function);
    std::optional<KeyCode> get_keycode(ScanCode scancode);
    void setup_input_devices(HWND hwnd);
    void handle_inputs(LPARAM lparam, HWND hwnd, Renderer& renderer);
    void keypress(KeyCode key, KeyState state);
    bool is_key_down(KeyCode key);
};

#endif
