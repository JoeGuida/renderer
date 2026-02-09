#include <print>

#include <spdlog/spdlog.h>

#include <renderer/extension.hpp>
#include <renderer/logger.hpp>
#include <renderer/renderer.hpp>
#include <window/window.hpp>

constexpr int width = 1280;
constexpr int height = 720;

#ifdef WINDOWS
int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
#elifdef APPLE
int main() {
#endif
    if(auto logger = init_logger(); logger != true) {
        std::println("could not initialize logger");
    }

#ifdef WINDOWS
    auto window_handle = initialize_window(instance, show_window, width, height, L"window class", L"renderer");
#elifdef APPLE
    auto window_handle = initialize_window(show_window, width, height, L"window class", L"renderer");
#endif
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
