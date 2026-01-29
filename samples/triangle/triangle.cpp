#if defined(_WIN32) || defined(_WIN64)

#include <spdlog/spdlog.h>

#include <renderer/extension.hpp>
#include <renderer/renderer.hpp>
#include <window/window.hpp>

constexpr int width = 1280;
constexpr int height = 720;

int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
    auto window_handle = initialize_window(instance, show_window, width, height, L"window class", L"renderer");
    if(!window_handle.has_value()) {
        spdlog::error(window_handle.error());
    }

    Window window {
        .width = width,
        .height = height,
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

    Renderer renderer;
    auto vk_context = init_renderer(renderer, window.handle.get(), instance, extensions);
    if(!vk_context.has_value()) {
        spdlog::error("could not get vk_context!");
    }

    std::function<void()> draw_callback = [&]() { draw(vk_context.value(), window.handle.get()); };

    run_window(window.handle.get(), draw_callback);
    destroy_context(vk_context.value());
}

#endif
