#include <array>
#include <expected>
#include <filesystem>
#include <iostream>
#include <string>
#include <print>

#include <windows.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

std::array<std::string, 73> keys {
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "Tilde",
    "Dash",
    "Equals",
    "BackSlash",
    "BackSpace",
    "Space",
    "Tab",
    "CapsLock",
    "Shift",
    "Control",
    "Alt",
    "Enter",
    "Escape",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "LeftBracket",
    "RightBracket",
    "UpArrow",
    "LeftArrow",
    "DownArrow",
    "RightArrow",
    "SemiColon",
    "Quote",
    "Comma",
    "Period",
    "Slash"
};

std::expected<void, std::string> init_logger() {
    try {
        auto logger = spdlog::basic_logger_mt("logger", "log/log.txt", true);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex& ex) {
        return std::unexpected(ex.what());
    }

    spdlog::info("logger: 'logger' initialized");
    return {};
}

int main() {
    if(auto logger = init_logger(); !logger.has_value()) {
        std::println("{}", logger.error());
        return EXIT_FAILURE;
    }

    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    INPUT_RECORD record;
    DWORD count;

    std::println("Press 'S' if a key does not exist on your keyboard");
    std::println();

    std::println("Start -------------------------------------------------------------------------");
    std::println();

    spdlog::info("Key,ScanCode,IsExtended");
    bool first = true;
    WORD skipcode = -1;

    for(const auto& key : keys) {
        std::print("Press {} -> ", key);

        while (true) {
            if (!ReadConsoleInput(input, &record, 1, &count)) {
                continue;
            }

            if (record.EventType != KEY_EVENT) {
                continue;
            }

            const auto& key_event = record.Event.KeyEvent;

            if (!key_event.bKeyDown) {
                continue;
            }

            if (key_event.wVirtualKeyCode == 0) {
                continue;
            }

            if(record.Event.KeyEvent.wVirtualKeyCode == 0x53 /* "S" */ && key != "S") {
                std::print("Skipped");
                continue;
            }
            else {
                bool is_extended = (key_event.dwControlKeyState & ENHANCED_KEY) != 0;
                spdlog::info("{},{},{}", key, key_event.wVirtualScanCode, is_extended);
                std::print("Done");
                break;
            }
        }

        std::println();
    }

    auto path = std::filesystem::current_path() / "log" / "log.txt";
    std::println();
    std::println("Finished ----------------------------------------------------------------------");
    std::println();

    std::println("Log file is located at {}", path.string());
    std::println("Press any key to exit");
    std::cin.get();

    return 0;
}
