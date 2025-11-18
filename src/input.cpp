#include "input.hpp"

#include <spdlog/spdlog.h>

bool is_key_down(const InputAction& action) {
    return action_to_state[action] == InputState::Down || action_to_state[action] == InputState::Pressed;
}

bool is_key_up(const KeyCode& key) {
    return action_to_state[key] == InputState::Up;
}

bool is_key_pressed(const KeyCode& key) {
    return action_to_state[key] == InputState::Down && previous_action_to_state[key] == InputState::Up;
}

bool is_key_released(const KeyCode& key) {
    return action_to_state[key] == InputState::Up && previous_action_to_state[key] == InputState::Down;
}

void bind(const InputBinding& binding) {
    bindings.push(binding);
}

