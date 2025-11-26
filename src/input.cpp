#include "input.hpp"

void Input::bind(const InputAction& action, const InputBinding& binding) {
    bindings[action].push_back(binding); 
}

InputState get_input_state(const KeyState& state, const InputState& last_state) {
    switch(state) {
        case KeyState::Up: {
            return (last_state == InputState::Down) ? InputState::Released : InputState::Up;
        }
        case KeyState::Down: {
            return (last_state == InputState::Up) ? InputState::Pressed : InputState::Down;
        }
        default: {
            return InputState::None;
        }
    }
}
