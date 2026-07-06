#include "input/key_state.hpp"

std::string to_string(KeyState state) {
    switch(state) {
        case KeyState::Up: {
            return "Up";
        }
        case KeyState::Down: {
            return "Down";
        }
        case KeyState::Pressed: {
            return "Pressed";
        }
        case KeyState::Released: {
            return "Released";
        }
    }

    return "Undefined";
}

