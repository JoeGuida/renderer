#ifndef INPUT_KEY_STATE_HPP
#define INPUT_KEY_STATE_HPP

#include <cstdint>
#include <string>

enum class KeyState : uint32_t {
    Pressed,
    Released,
    Up,
    Down
};

struct InputState {
    KeyState state;
    KeyState previous_state;
    bool toggled;
};

std::string to_string(KeyState state);

#endif

