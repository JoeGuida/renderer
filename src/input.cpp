#include "input.hpp"

void Input::bind(const InputAction& action, const InputBinding& binding) {
    bindings[action].push_back(binding); 
}

