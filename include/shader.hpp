#ifndef RENDERER_SHADER_HPP
#define RENDERER_SHADER_HPP

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

enum class ShaderStage : uint32_t {
    Vertex = VK_SHADER_STAGE_VERTEX_BIT,
    Fragment = VK_SHADER_STAGE_FRAGMENT_BIT,
    Geometry = VK_SHADER_STAGE_GEOMETRY_BIT,
    Compute = VK_SHADER_STAGE_COMPUTE_BIT
};

struct Shader {
    std::filesystem::path filepath;
    ShaderStage stage;
    VkShaderModule module;
};

std::vector<char> read_file(const std::filesystem::path& filepath);
void create_shader_module(VkDevice device, Shader& shader, const std::vector<char>& code);

#endif
