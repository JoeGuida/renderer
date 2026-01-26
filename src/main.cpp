#include <platform/platform.hpp>

#include "logger.hpp"

#include "samples/triangle_sample.hpp"

#if defined(_WIN64) || defined(_WIN32)

int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
    init_logger();
    triangle_sample(instance, show_window, 1280, 720);
}

#endif
