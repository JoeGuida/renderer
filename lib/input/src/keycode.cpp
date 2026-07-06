#include "input/keycode.hpp"

std::string to_string(KeyCode keycode) {
    switch(keycode) {
        case KeyCode::A: {
            return "A";
        }
        case KeyCode::B: {
            return "B";
        }
        case KeyCode::C: {
            return "C";
        }
        case KeyCode::D: {
            return "D";
        }
        case KeyCode::E: {
            return "E";
        }
        case KeyCode::F: {
            return "F";
        }
        case KeyCode::G: {
            return "G";
        }
        case KeyCode::H: {
            return "H";
        }
        case KeyCode::I: {
            return "I";
        }
        case KeyCode::J: {
            return "J";
        }
        case KeyCode::K: {
            return "K";
        }
        case KeyCode::L: {
            return "L";
        }
        case KeyCode::M: {
            return "M";
        }
        case KeyCode::N: {
            return "N";
        }
        case KeyCode::O: {
            return "O";
        }
        case KeyCode::P: {
            return "P";
        }
        case KeyCode::Q: {
            return "Q";
        }
        case KeyCode::R: {
            return "R";
        }
        case KeyCode::S: {
            return "S";
        }
        case KeyCode::T: {
            return "T";
        }
        case KeyCode::U: {
            return "U";
        }
        case KeyCode::V: {
            return "V";
        }
        case KeyCode::W: {
            return "W";
        }
        case KeyCode::X: {
            return "X";
        }
        case KeyCode::Y: {
            return "Y";
        }
        case KeyCode::Z: {
            return "Z";
        }
        case KeyCode::N0: {
            return "0";
        }
        case KeyCode::N1: {
            return "1";
        }
        case KeyCode::N2: {
            return "2";
        }
        case KeyCode::N3: {
            return "3";
        }
        case KeyCode::N4: {
            return "4";
        }
        case KeyCode::N5: {
            return "5";
        }
        case KeyCode::N6: {
            return "6";
        }
        case KeyCode::N7: {
            return "7";
        }
        case KeyCode::N8: {
            return "8";
        }
        case KeyCode::N9: {
            return "9";
        }
        case KeyCode::Tilde: {
            return "`";
        }
        case KeyCode::Minus: {
            return "-";
        }
        case KeyCode::Equals: {
            return "=";
        }
        case KeyCode::BackSlash: {
            return "\\";
        }
        case KeyCode::BackSpace: {
            return "BackSpace";
        }
        case KeyCode::Space: {
            return "Space";
        }
        case KeyCode::Tab: {
            return "Tab";
        }
        case KeyCode::Caps: {
            return "Caps";
        }
        case KeyCode::LeftShift: {
            return "LeftShift";
        }
        case KeyCode::Control: {
            return "Control";
        }
        case KeyCode::Alt: {
            return "Alt";
        }
        case KeyCode::RightShift: {
            return "RightShift";
        }
        case KeyCode::Enter: {
            return "Enter";
        }
        case KeyCode::Escape: {
            return "Escape";
        }
        case KeyCode::F1: {
            return "F1";
        }
        case KeyCode::F2: {
            return "F2";
        }
        case KeyCode::F3: {
            return "F3";
        }
        case KeyCode::F4: {
            return "F4";
        }
        case KeyCode::F5: {
            return "F5";
        }
        case KeyCode::F6: {
            return "F6";
        }
        case KeyCode::F7: {
            return "F7";
        }
        case KeyCode::F8: {
            return "F8";
        }
        case KeyCode::F9: {
            return "F9";
        }
        case KeyCode::F10: {
            return "F10";
        }
        case KeyCode::F11: {
            return "F11";
        }
        case KeyCode::F12: {
            return "F12";
        }
        case KeyCode::LeftBracket: {
            return "[";
        }
        case KeyCode::RightBracket: {
            return "]";
        }
        case KeyCode::UpArrow: {
            return "UpArrow";
        }
        case KeyCode::LeftArrow: {
            return "LeftArrow";
        }
        case KeyCode::DownArrow: {
            return "DownArrow";
        }
        case KeyCode::RightArrow: {
            return "RightArrow";
        }
        case KeyCode::SemiColon: {
            return ";";
        }
        case KeyCode::Quote: {
            return "'";
        }
        case KeyCode::Comma: {
            return ",";
        }
        case KeyCode::Period: {
            return ".";
        }
        case KeyCode::Slash: {
            return "/";
        }
    };

    return "Undefined";
}

