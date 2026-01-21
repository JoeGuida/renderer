#include <functional>
#include <vector>

#include <platform/platform.hpp>
#include <window/window.hpp>

#include "api_vulkan.hpp"
#include "extension.hpp"
#include "logger.hpp"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

#if defined(_WIN64) || defined(_WIN32)

int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
    init_logger();

    auto window_handle = initialize_window(instance, show_window, SCREEN_WIDTH, SCREEN_HEIGHT, L"window class", L"renderer");
    if(!window_handle.has_value()) {
        return EXIT_FAILURE;
    }

    Window window {
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .handle = std::move(window_handle.value())
    };

    RendererExtensions extensions = {
        .instance = {
            "VK_EXT_debug_utils",
            "VK_KHR_surface",
            "VK_KHR_win32_surface" 
        },
        .device = { "VK_KHR_swapchain" },
        .validation = { "VK_LAYER_KHRONOS_validation" }
    };

    auto vk_context = init_renderer(window.handle.get(), instance, extensions);
    if(!vk_context.has_value()) {
        spdlog::error("could not get vk_context!");
        return EXIT_FAILURE;
    }

    std::function<void()> draw_callback = [&]() { draw(vk_context.value()); };

    run_window(window.handle.get(), draw_callback);
    destroy_context(vk_context.value());

    return EXIT_SUCCESS;
}

#endif
