#ifndef RENDERER_SHADER_HPP
#define RENDERER_SHADER_HPP

#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

inline std::vector<char> read_file(const std::filesystem::path& filepath) {
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

#endif
