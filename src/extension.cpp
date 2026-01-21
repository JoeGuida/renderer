#include "extension.hpp"

std::unordered_set<std::string> get_supported_instance_extensions() {
    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

    std::vector<VkExtensionProperties> extensions(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());

    std::unordered_set<std::string> supported_extensions;
    for(const auto& extension : extensions) {
        supported_extensions.insert(std::string(extension.extensionName));
    }

    return supported_extensions;
}

std::unordered_set<std::string> get_supported_device_extensions(VkPhysicalDevice physical_device) {
    uint32_t device_extension_count = 0;
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_extension_count, nullptr);

    std::vector<VkExtensionProperties> device_extensions(device_extension_count);
    vkEnumerateDeviceExtensionProperties(physical_device, nullptr, &device_extension_count, device_extensions.data());

    std::unordered_set<std::string> supported_extensions;
    for(const auto& extension : device_extensions) {
        supported_extensions.insert(std::string(extension.extensionName));
    }

    return supported_extensions;
}

bool instance_extensions_supported(const std::vector<const char*>& extensions) {
    auto supported = get_supported_instance_extensions();
    for(const auto& extension : extensions) {
        if(!supported.contains(extension)) {
            return false;
        }
    }

    return true;
}

bool device_extensions_supported(VkPhysicalDevice physical_device, const std::vector<const char*> extensions) {
    auto supported = get_supported_device_extensions(physical_device);
    for(const auto& extension : extensions) {
        if(!supported.contains(extension)) {
            return false;
        }
    }

    return true;
}

bool validation_layers_available(const std::vector<const char*>& validation_layers) {
    uint32_t layer_count;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

    std::vector<VkLayerProperties> available_layers(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    for(const auto& layer : validation_layers) {
        bool layer_found = false;

        for(const auto& layer_properties : available_layers) {
            if(strcmp(layer, layer_properties.layerName) == 0) {
                layer_found = true;
                break;
            }
        }

        if(!layer_found) {
            return false;
        }
    }

    return true;
}
