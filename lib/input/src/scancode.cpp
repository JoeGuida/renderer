#include "input/scancode.hpp"

std::string to_string(ScanCode scancode) {
    switch(scancode) {
        case ScanCode::A: {
            return "A";
        }
        case ScanCode::B: {
            return "B";
        }
        case ScanCode::C: {
            return "C";
        }
        case ScanCode::D: {
            return "D";
        }
        case ScanCode::E: {
            return "E";
        }
        case ScanCode::F: {
            return "F";
        }
        case ScanCode::G: {
            return "G";
        }
        case ScanCode::H: {
            return "H";
        }
        case ScanCode::I: {
            return "I";
        }
        case ScanCode::J: {
            return "J";
        }
        case ScanCode::K: {
            return "K";
        }
        case ScanCode::L: {
            return "L";
        }
        case ScanCode::M: {
            return "M";
        }
        case ScanCode::N: {
            return "N";
        }
        case ScanCode::O: {
            return "O";
        }
        case ScanCode::P: {
            return "P";
        }
        case ScanCode::Q: {
            return "Q";
        }
        case ScanCode::R: {
            return "R";
        }
        case ScanCode::S: {
            return "S";
        }
        case ScanCode::T: {
            return "T";
        }
        case ScanCode::U: {
            return "U";
        }
        case ScanCode::V: {
            return "V";
        }
        case ScanCode::W: {
            return "W";
        }
        case ScanCode::X: {
            return "X";
        }
        case ScanCode::Y: {
            return "Y";
        }
        case ScanCode::Z: {
            return "Z";
        }
        case ScanCode::N0: {
            return "0";
        }
        case ScanCode::N1: {
            return "1";
        }
        case ScanCode::N2: {
            return "2";
        }
        case ScanCode::N3: {
            return "3";
        }
        case ScanCode::N4: {
            return "4";
        }
        case ScanCode::N5: {
            return "5";
        }
        case ScanCode::N6: {
            return "6";
        }
        case ScanCode::N7: {
            return "7";
        }
        case ScanCode::N8: {
            return "8";
        }
        case ScanCode::N9: {
            return "9";
        }
        case ScanCode::Tilde: {
            return "`";
        }
        case ScanCode::Minus: {
            return "-";
        }
        case ScanCode::Equals: {
            return "=";
        }
        case ScanCode::BackSlash: {
            return "\\";
        }
        case ScanCode::BackSpace: {
            return "BackSpace";
        }
        case ScanCode::Space: {
            return "Space";
        }
        case ScanCode::Tab: {
            return "Tab";
        }
        case ScanCode::Caps: {
            return "Caps";
        }
        case ScanCode::LeftShift: {
            return "LeftShift";
        }
        case ScanCode::Control: {
            return "Control";
        }
        case ScanCode::Alt: {
            return "Alt";
        }
        case ScanCode::RightShift: {
            return "RightShift";
        }
        case ScanCode::Enter: {
            return "Enter";
        }
        case ScanCode::Escape: {
            return "Escape";
        }
        case ScanCode::F1: {
            return "F1";
        }
        case ScanCode::F2: {
            return "F2";
        }
        case ScanCode::F3: {
            return "F3";
        }
        case ScanCode::F4: {
            return "F4";
        }
        case ScanCode::F5: {
            return "F5";
        }
        case ScanCode::F6: {
            return "F6";
        }
        case ScanCode::F7: {
            return "F7";
        }
        case ScanCode::F8: {
            return "F8";
        }
        case ScanCode::F9: {
            return "F9";
        }
        case ScanCode::F10: {
            return "F10";
        }
        case ScanCode::F11: {
            return "F11";
        }
        case ScanCode::F12: {
            return "F12";
        }
        case ScanCode::LeftBracket: {
            return "[";
        }
        case ScanCode::RightBracket: {
            return "]";
        }
        case ScanCode::UpArrow: {
            return "UpArrow";
        }
        case ScanCode::LeftArrow: {
            return "LeftArrow";
        }
        case ScanCode::DownArrow: {
            return "DownArrow";
        }
        case ScanCode::RightArrow: {
            return "RightArrow";
        }
        case ScanCode::SemiColon: {
            return ";";
        }
        case ScanCode::Quote: {
            return "'";
        }
        case ScanCode::Comma: {
            return ",";
        }
        case ScanCode::Period: {
            return ".";
        }
        case ScanCode::Slash: {
            return "/";
        }
    };

    return "Undefined";
}

