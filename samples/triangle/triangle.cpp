#include <array>
#include <filesystem>
#include <print>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "input/input.hpp"
#include "renderer/api/gl_loader.hpp"
#include "renderer/core/renderer.hpp"
#include "renderer/types/uint.hpp"
#include "renderer/types/vec.hpp"
#include "window/window.hpp"

std::expected<void, std::string> init_logger() {
    try {
        auto logger = spdlog::basic_logger_mt("logger", (std::filesystem::current_path() / "log" / "log.txt").string(), true);
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

int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
#ifdef DEBUG
    // sometimes tools like RenderDoc need to be attached to the process before opengl is initialized
    MessageBoxA(nullptr, "Continue?", "Continue?", MB_OK);
#endif

    if(auto logger = init_logger(); !logger.has_value()) {
        std::println("{}", logger.error());
    }

    // Renderer / Window Setup ---------------------------------------------------------------------
    u32 width = 1280;
    u32 height = 720;

    Input input;

    auto window_result = initialize_window(instance, show_window, width, height, L"window class", L"Triangle", &input);
    if(!window_result.has_value()) {
        spdlog::error("error initializing window!");
        return EXIT_FAILURE;
    }
    auto window(std::move(window_result.value()));

    if(auto opengl = initialize_opengl(window.handle.get()); !opengl.has_value()) {
        spdlog::error("error initializing opengl!");
        return EXIT_FAILURE;
    }

    // Triangle -----------------------------------------------------------------------------------

    std::array<vec3, 6> vertices {
        vec3(-0.5f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.5f, 0.0f), vec3(0.0f, 0.0f, 1.0f)
    };

    u32 vao;
    u32 vbo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)(sizeof(vec3)));
    glEnableVertexAttribArray(1);

    // Input --------------------------------------------------------------------------------------

    setup_input_devices(input, window.handle->hwnd);

    InputBinding close_window {
        .keycode = KeyCode::Escape,
        .action = InputAction::CallOnce,
        .trigger = KeyState::Pressed,
        .callback = [&window](){ DestroyWindow(window.handle->hwnd); }
    };

    InputBinding printme {
        .keycode = KeyCode::D,
        .action = InputAction::CallOnce,
        .trigger = KeyState::Down,
        .callback = [&window](){ spdlog::info("Called printme"); }
    };

    bind_input(input, close_window);
    bind_input(input, printme);

    // Main Loop -----------------------------------------------------------------------------------
    spdlog::info("running window");
    run_window(window.handle.get(), [](){});

    return EXIT_SUCCESS;
}

