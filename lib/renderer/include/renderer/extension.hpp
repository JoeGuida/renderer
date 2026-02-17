#ifndef RENDERER_EXTENSION_HPP
#define RENDERER_EXTENSION_HPP

#include <string>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "device.hpp"
#include "renderer_extensions.hpp"

struct RendererFeatures {
    bool debug;
    bool validation;
    bool presentation;
};

enum class Extension : uint32_t {
    DebugUtils,
    Validation,
    Swapchain,
    Surface,
    Win32Surface
};

std::string to_string(Extension extension);
std::unordered_set<std::string> get_supported_instance_extensions();
std::unordered_set<std::string> get_supported_device_extensions(const PhysicalDevice& device);
bool instance_extensions_supported(const RendererExtensions& extensions);
bool device_extensions_supported(const PhysicalDevice& device, const RendererExtensions& extensions);
bool validation_layers_available(const RendererExtensions& extensions);
RendererExtensions get_renderer_extensions(const RendererFeatures& features);

#endif
