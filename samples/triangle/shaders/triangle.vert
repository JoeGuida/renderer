#version 460 core

const uint N = 64;
const uint P = 3;

layout (location = 0) in vec3 pos;

layout (std140, binding = 0) uniform vertex_data {
    vec4 positions[N * P];
    vec4 scales[N * P];
    vec4 colors[N * P];
};

out vec4 vertex_color;

uniform uint offset;

void main() {
    gl_Position = vec4(pos, 0.0) + positions[gl_InstanceID + offset]; // positions[i].w will always be 1
    vertex_color = colors[gl_InstanceID + offset];
}
