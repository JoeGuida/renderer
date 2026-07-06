#ifndef INPUT_BINDING_HPP
#define INPUT_BINDING_HPP

#include <functional>

#include "input/action.hpp"
#include "input/keycode.hpp"
#include "input/key_state.hpp"

struct InputBinding {
    KeyCode keycode;
    InputAction action;
    KeyState trigger;
    std::function<void()> callback;
};

#endif
