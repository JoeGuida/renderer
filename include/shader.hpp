#ifndef RENDERER_SHADER_HPP
#define RENDERER_SHADER_HPP

#include <filesystem>
#include <vector>

#include <vulkan/vulkan.hpp>

std::vector<char> read_file(const std::filesystem::path& filepath);
VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& code);

#endif
