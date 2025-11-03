#ifndef GL_RENDERER_RENDERER_HPP
#define GL_RENDERER_RENDERER_HPP

#include <array>
#include <cstdint>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "window.hpp"

constexpr size_t OBJECT_COUNT = 256;
constexpr size_t INDEX_COUNT = 36;
constexpr size_t VERTEX_COUNT = 24;

constexpr size_t POSITIONS_SIZE = OBJECT_COUNT * sizeof(glm::vec4);
constexpr size_t COLORS_SIZE = OBJECT_COUNT * sizeof(glm::vec4);
constexpr size_t SIZES_SIZE = OBJECT_COUNT * sizeof(glm::vec4);
constexpr size_t UBO_SIZE = POSITIONS_SIZE + COLORS_SIZE + SIZES_SIZE;

struct Renderer {
    uint32_t vao;
    uint32_t vbo;
    uint32_t ebo;
    uint32_t ubo;

    size_t count;

    uint32_t shader;

    std::array<glm::vec3, VERTEX_COUNT> vertices = {
        glm::vec3(-1.0f,  1.0f,  1.0f), // Front
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f), 
        glm::vec3( 1.0f, -1.0f,  1.0f), 
        glm::vec3( 1.0f,  1.0f,  1.0f), // Right
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f), // Left
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f, -1.0f), // Back
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, -1.0f), // Top
        glm::vec3( 1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f,  1.0f),
        glm::vec3( 1.0f,  1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f,  1.0f), // Bottom
        glm::vec3( 1.0f, -1.0f,  1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3( 1.0f, -1.0f, -1.0f)
    };

    std::array<uint32_t, INDEX_COUNT> indices = {
         0,  2,  1,  1,  2,  3,
         4,  6,  5,  5,  6,  7,
         8, 10,  9,  9, 10, 11,
        12, 14, 13, 13, 14, 15,
        16, 18, 17, 17, 18, 19,
        20, 22, 21, 21, 22, 23,
    };

    std::array<glm::vec4, OBJECT_COUNT> positions;
    std::array<glm::vec4, OBJECT_COUNT> colors;
    std::array<glm::vec4, OBJECT_COUNT> sizes;
};

void init(Renderer& renderer);
void run_message_loop(Window& window, Renderer& renderer);
void draw(const Renderer& renderer);
void setup(const Renderer& renderer);

#endif
