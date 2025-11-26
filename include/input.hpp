#ifndef RENDERER_INPUT_HPP
#define RENDERER_INPUT_HPP

#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#if defined(_WIN64) || defined(_WIN32)
    #include <platform/win32_input.hpp>
#elif defined(__linux__)
    #include <platform/linux_input.hpp>
#endif

#include "input_codes.hpp"

struct PlatformInput;

enum class InputAction : uint32_t {
    MoveForward,
    MoveLeft,
    MoveBack,
    MoveRight,
    Quit
};

enum class InputState : uint32_t {
    Up,
    Down,
    Pressed,
    Released
};

struct InputBinding {
    InputState state;
    std::function<void()> callback;
};

inline const std::unordered_map<InputAction, std::vector<ScanCode>> default_key_bindings {
    { InputAction::MoveForward, { ScanCode::W, ScanCode::ArrowUp    } },
    { InputAction::MoveBack,    { ScanCode::S, ScanCode::ArrowDown  } },
    { InputAction::MoveLeft,    { ScanCode::A, ScanCode::ArrowLeft  } },
    { InputAction::MoveRight,   { ScanCode::D, ScanCode::ArrowRight } },
    { InputAction::Quit,        { ScanCode::Esc } },
};

struct Input {
    std::unique_ptr<PlatformInput> platform_input;
    std::unordered_map<ScanCode, KeyState> key_states;
    std::unordered_map<InputAction, std::vector<ScanCode>> keys = default_key_bindings;
    std::unordered_map<InputAction, std::vector<InputBinding>> bindings;

    void bind(const InputAction& action, const InputBinding& binding);
};

#endif
