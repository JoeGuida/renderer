#include "renderer.hpp"

#include <glm/vec4.hpp>
#include <opengl/glcorearb.h>

#include "gl_loader.hpp"
#include "shader.hpp"

void init(Renderer& renderer) {
    glGenVertexArrays(1, &renderer.vao);
    glGenBuffers(1, &renderer.vbo);
    glGenBuffers(1, &renderer.ebo);
    glGenBuffers(1, &renderer.ubo);

    for(int i = 0; i < OBJECT_COUNT; i++) {
        renderer.positions[i] = glm::vec4(0.0f);
        renderer.colors[i] = glm::vec4(0.0f);
        renderer.sizes[i] = glm::vec4(0.0f);
    }

    renderer.count = 0;
}

void draw(const Renderer& renderer, int width, int height) {
    glViewport(0, 0, width, height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(renderer.shader);
    glDrawElementsInstanced(GL_TRIANGLES, renderer.indices.size(), GL_UNSIGNED_INT, 0, renderer.count); 
}

void setup(const Renderer& renderer) {
    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer.vbo);
    glBufferData(GL_ARRAY_BUFFER, renderer.vertices.size() * sizeof(glm::vec3), renderer.vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderer.indices.size() * sizeof(uint32_t), renderer.indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, renderer.ubo);
    glBufferData(GL_UNIFORM_BUFFER, UBO_SIZE, nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, POSITIONS_SIZE, renderer.positions.data());
    glBufferSubData(GL_UNIFORM_BUFFER, POSITIONS_SIZE, COLORS_SIZE, renderer.colors.data());
    glBufferSubData(GL_UNIFORM_BUFFER, POSITIONS_SIZE + COLORS_SIZE, SIZES_SIZE, renderer.sizes.data());
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, renderer.ubo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
}

void update_positions(Renderer& renderer, uint32_t id, const glm::vec4& direction) {
    renderer.positions[id] += direction;

    // udpate buffers
    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_UNIFORM_BUFFER, renderer.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, POSITIONS_SIZE, renderer.positions.data());

}

void bind_vao(uint32_t vao) {
    glBindVertexArray(vao);
}

