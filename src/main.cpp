#include <utility>

#include <platform/platform.hpp>
#include <window/window.hpp>

#include "api_vulkan.hpp"
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

    std::vector<const char*> validation_layers {
        "VK_LAYER_KHRONOS_validation"
    };

    std::vector<const char*> instance_extensions {
        "VK_EXT_debug_utils",
        "VK_KHR_surface",
        "VK_KHR_win32_surface"
    };

    std::vector<const char*> device_extensions {
        "VK_KHR_swapchain"
    };

    auto vk_context = vk_init(window.handle.get(), instance, validation_layers, instance_extensions, device_extensions);
    if(!vk_context.has_value()) {
        return EXIT_FAILURE;
    }

    std::function<void()> draw = [&]() { vk_draw(vk_context.value()); };

    run_window(window.handle.get(), draw);
    vk_cleanup(vk_context.value());

    return EXIT_SUCCESS;
}

#endif
