#ifndef INPUT_INPUT_HPP
#define INPUT_INPUT_HPP

#include <queue>
#include <unordered_map>
#include <vector>

#include <Windows.h>

#include "input/binding.hpp"
#include "input/keycode.hpp"
#include "input/key_state.hpp"
#include "input/scancode.hpp"

struct Input;

std::unordered_map<ScanCode, KeyCode> default_key_mapping();
std::unordered_map<KeyCode, InputState> default_key_states();

void bind_input(Input& input, const InputBinding& binding);
void handle_inputs(Input& input, LPARAM lparam);
void input_update(Input& input);
void keyboard_input(Input& input, RAWKEYBOARD keyboard);
void mouse_input(Input& input, RAWMOUSE mouse);
void remap(Input& input, KeyCode keycode, ScanCode scancode);
void setup_input_devices(Input& input, HWND hwnd);

struct Input {
    std::unordered_map<ScanCode, KeyCode> key_mapping;
    std::unordered_map<KeyCode, InputState> key_states;
    std::queue<KeyCode> keys;
    std::vector<InputBinding> bindings;
    bool initialized = false;

    Input() : key_mapping(default_key_mapping()), key_states(default_key_states()) {}
};

#endif

