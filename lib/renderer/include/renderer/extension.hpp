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

struct RendererFeatures {
    bool debug;
    bool validation;
    bool presentation;
};

std::unordered_set<std::string> get_supported_instance_extensions();
std::unordered_set<std::string> get_supported_device_extensions(VkPhysicalDevice physical_device);
bool instance_extensions_supported(const RendererExtensions& extensions);
bool device_extensions_supported(VkPhysicalDevice physical_device, const RendererExtensions& extensions);
bool validation_layers_available(const RendererExtensions& extensions);
RendererExtensions get_renderer_extensions(const RendererFeatures& features);

#endif
