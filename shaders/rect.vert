#version 430 core

layout (location = 0) in vec3 pos;

const uint SIZE = 256;

layout (std140, binding = 0) uniform instance_data {
    vec4 positions[SIZE];
    vec4 colors[SIZE];
    vec4 sizes[SIZE];
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vertex_color;

void main() {
    vec3 instance_pos = positions[gl_InstanceID].xyz;
    float size = sizes[gl_InstanceID].x;
    gl_Position = projection * view * model * vec4(pos * size + instance_pos, 1.0);
    vertex_color = colors[gl_InstanceID].xyz;
}

