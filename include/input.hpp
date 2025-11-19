#ifndef GL_RENDERER_INPUT_HPP
#define GL_RENDERER_INPUT_HPP

#if defined(_WIN64) || defined(_WIN32)
    #include <platform/win32_input.hpp>
#elif defined(__linux__)
    #include <platform/linux_input.hpp>
#endif

#include <cstdint>
#include <functional>
#include <unordered_map>

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

inline std::unordered_map<InputAction, InputState> states;
inline std::unordered_map<InputAction, InputState> last_states;
inline std::unordered_map<InputAction, std::function<void()>> callbacks;

struct Input {
    bool is_key_down(const KeyCode& key);
    bool is_key_up(const KeyCode& key);
    bool is_key_pressed(const KeyCode& key);
    bool is_key_released(const KeyCode& key);
};

#endif
