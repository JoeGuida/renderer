#include "renderer.hpp"

#include <glm/vec4.hpp>
#include <opengl/glcorearb.h>

#include "gl_loader.hpp"

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

void run_message_loop(HWND hwnd, HDC hdc, Renderer& renderer) {
    MSG message;
    ZeroMemory(&message, sizeof(MSG));
    while (true) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT) {
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else {
            RECT client_rect;
            GetClientRect(hwnd, &client_rect);

            int client_width = client_rect.right - client_rect.left;
            int client_height = client_rect.bottom - client_rect.top;

            glViewport(0, 0, client_width, client_height);
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(renderer.vao);

            draw(renderer); 

            SwapBuffers(hdc);
        }
    }
}

void draw(const Renderer& renderer) {
//    time_point now = std::chrono::system_clock::now();
//    renderer.clock.delta_time = now - renderer.clock.last_frame_time;
//    renderer.clock.last_frame_time = now;
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

void update_array_buffer(Renderer& renderer) {
    glBindVertexArray(renderer.vao);
    glBindBuffer(GL_UNIFORM_BUFFER, renderer.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, POSITIONS_SIZE, renderer.positions.data());
}

