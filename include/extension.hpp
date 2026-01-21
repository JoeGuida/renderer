#ifndef RENDERER_EXTENSION_HPP
#define RENDERER_EXTENSION_HPP

#include <string>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.hpp>

struct RendererExtensions {
    std::vector<const char*> instance;
    std::vector<const char*> device;
    std::vector<const char*> validation;
};

std::unordered_set<std::string> get_supported_instance_extensions();
std::unordered_set<std::string> get_supported_device_extensions(VkPhysicalDevice physical_device);
bool instance_extensions_supported(const std::vector<const char*>& extensions);
bool device_extensions_supported(VkPhysicalDevice physical_device, const std::vector<const char*> extensions);

#endif
