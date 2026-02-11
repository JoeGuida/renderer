#include <print>

#include <spdlog/spdlog.h>

#include <renderer/extension.hpp>
#include <renderer/logger.hpp>
#include <renderer/renderer.hpp>
#include <window/window.hpp>

constexpr int width = 1280;
constexpr int height = 720;

int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
    if(auto logger = init_logger(); logger != true) {
        std::println("could not initialize logger");
    }

    auto window_handle = initialize_window(instance, show_window, width, height, L"window class", L"renderer");
    if(!window_handle.has_value()) {
        spdlog::error(window_handle.error());
    }

    Window window {
        .width = width,
        .height = height,
        .handle = std::move(window_handle.value())
    };

    RendererFeatures features {
        .debug = true,
        .validation = true,
        .presentation = true
    };

    Renderer renderer;
    auto context = init_renderer(renderer, window.handle.get(), instance, features);
    if(!context.has_value()) {
        spdlog::error(context.error());
    }

    std::function<void()> draw_callback = [&]() { draw(context.value(), window.handle.get()); };

    run_window(window.handle.get(), draw_callback);
    destroy_context(context.value());
}
