#include "input.hpp"

#include <spdlog/spdlog.h>

bool is_key_down(const KeyCode& key) {
    return key_states[key] == KeyState::Down;
}

bool is_key_up(const KeyCode& key) {
    return key_states[key] == KeyState::Up;
}

bool is_key_pressed(const KeyCode& key) {
    return key_states[key] == KeyState::Down && last_states[key] == KeyState::Up;
}

bool is_key_released(const KeyCode& key) {
    return key_states[key] == KeyState::Up && last_states[key] == KeyState::Down;
}

