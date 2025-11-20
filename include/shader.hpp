#ifndef RENDERER_SHADER_HPP
#define RENDERER_SHADER_HPP

#include <cstdint>
#include <expected>
#include <filesystem>
#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

std::expected<uint32_t, std::string> compile_shader(const std::filesystem::path& source_folder, const std::string& name, uint32_t type);
std::expected<uint32_t, std::string> link_shaders(uint32_t vertex_shader, uint32_t fragment_shader);
void set_shader_uniform(uint32_t program, const std::string& uniform, float value);
void set_shader_uniform(uint32_t program, const std::string& uniform, const glm::mat4x4& value);
void set_shader_uniform(uint32_t program, const std::string& uniform, const glm::vec3& value);

#endif

