#include <filesystem>
#include <functional>
#include <memory>
#include <print>

#include "platform.hpp"

#include <GL/gl.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

#include "camera.hpp"
#include "convert.hpp"
#include "gl_loader.hpp"
#include "input.hpp"
#include "logger.hpp"
#include "renderer.hpp"
#include "shader.hpp"
#include "window.hpp"

constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr float ASPECT_RATIO = SCREEN_WIDTH / static_cast<float>(SCREEN_HEIGHT);

constexpr glm::vec3 WORLD_UP(0.0f, 1.0f, 0.0f);
constexpr glm::vec3 WORLD_FRONT(0.0f, 0.0f, -1.0f);
constexpr glm::vec3 WORLD_RIGHT(1.0f, 0.0f, 0.0f);

void handle_error(const std::expected<uint32_t, std::string>& expect) {
    if(!expect.has_value()) {
        spdlog::error("error :: {}", expect.error());
    }
}

int WinMain(HINSTANCE instance, HINSTANCE unused, LPSTR command_line, int show_window) {
#if defined(DEBUG) || defined(_DEBUG)
    MessageBoxW(nullptr, L"continue?", L"continue?", MB_ICONQUESTION | MB_OK);
#endif

    if(!init_logger()) {
        std::println("error initializing logger");
        return -1;
    }

    Camera camera {
        .position = glm::vec3(0.0f, 0.0f, 1.0f),
        .direction = glm::vec3(0.0f, 0.0f, -1.0f),
        .up = WORLD_UP,
        .front = WORLD_FRONT,
        .right = WORLD_RIGHT,
        .fov_radians = glm::radians(90.0f),
        .aspect = ASPECT_RATIO,
        .near = 0.01f,
        .far = 10.0f
    };

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();

    auto initialized_window = initialize_window(instance, show_window, SCREEN_WIDTH, SCREEN_HEIGHT, L"class_name", L"Renderer");
    if(!initialized_window.has_value()) {
        spdlog::error("error initializing window :: {}", initialized_window.error());
    }
    Window window = std::move(initialized_window.value());
    init(*renderer);

    // cube
    glm::vec3 pos(0.0f, 0.0f, -1.0f);
    glm::vec3 color(0.15f, 0.15f, 0.15f);
    float size = 0.2f;

    renderer->positions[renderer->count] = to_vec4(pos);
    renderer->colors[renderer->count] = to_vec4(color);
    renderer->sizes[renderer->count] = to_vec4(size);
    renderer->count++;

    auto vertex_shader = compile_shader(std::filesystem::current_path() / "shaders", "rect", GL_VERTEX_SHADER);
    auto fragment_shader = compile_shader(std::filesystem::current_path() / "shaders", "rect", GL_FRAGMENT_SHADER);

    handle_error(vertex_shader);
    handle_error(fragment_shader);

    auto shader_program = link_shaders(vertex_shader.value(), fragment_shader.value());
    handle_error(shader_program);

    glm::mat4 model(1.0f);
    glm::mat4 view = glm::perspective(camera.fov_radians, camera.aspect, camera.near, camera.far);
    glm::mat4 projection = projection_matrix(camera); 

    glUseProgram(shader_program.value());
    set_shader_uniform(shader_program.value(), "model", model);
    set_shader_uniform(shader_program.value(), "view", view);
    set_shader_uniform(shader_program.value(), "projection", projection);
    renderer->shader = shader_program.value();

    setup(*renderer);
    run_message_loop(window.platform_window.get(), renderer.get());

    return 0;
}

