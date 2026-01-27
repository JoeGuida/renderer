#include <renderer/shader.hpp>

#include <fstream>

std::vector<char> read_file(const std::filesystem::path& filepath) {
    std::ifstream file(filepath.string(), std::ios::ate | std::ios::binary);

    if(!file.is_open()) {
        throw std::runtime_error(std::format("could not open file : {}", filepath.string()));
    }

    size_t file_size = file.tellg();
    std::vector<char> buffer(file_size);
    file.seekg(0);
    file.read(buffer.data(), file_size);
    file.close();

    return buffer;
}

void create_shader_module(VkDevice device, Shader& shader, const std::vector<char>& code) {
    assert(code.size() % 4 == 0);

    VkShaderModuleCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code.size(),
        .pCode = reinterpret_cast<const uint32_t*>(code.data())
    };

    if(vkCreateShaderModule(device, &create_info, nullptr, &shader.module) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module");
    }
}
