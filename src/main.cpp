#include <platform/platform.hpp>
#include <window/window.hpp>

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;

#if defined(_WIN64) || defined(_WIN32)

#include <platform/windows/win32_window.hpp>

int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
    auto window_handle = initialize_window(instance, show_window, SCREEN_WIDTH, SCREEN_HEIGHT, L"window class", L"renderer");
    if(!window_handle.has_value()) {
        return EXIT_FAILURE;
    }

    Window window {
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .handle = std::move(window_handle.value())
    };

    run_window(window.handle.get());

    return EXIT_SUCCESS;
}

#else

int main() {
    return EXIT_SUCCESS;
}

#endif
