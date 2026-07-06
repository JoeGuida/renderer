#ifndef INPUT_ACTION_HPP
#define INPUT_ACTION_HPP

#include <cstdint>

enum class InputAction : uint32_t {
    CallOnce,
    Repeat,
    Toggle
};

#endif
