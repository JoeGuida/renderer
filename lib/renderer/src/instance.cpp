#include <renderer/instance.hpp>

VkInstance create_instance(const RendererExtensions& extensions) {
    VkInstance instance;

    VkApplicationInfo application_info {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Renderer",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "EngineName",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0
    };

    VkInstanceCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &application_info,
        .enabledLayerCount = static_cast<uint32_t>(extensions.validation.size()),
        .ppEnabledLayerNames = extensions.validation.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extensions.instance.size()),
        .ppEnabledExtensionNames = extensions.instance.data()
    };

    VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
    if(result != VK_SUCCESS) {
        throw std::runtime_error("error creating vk instance");
    }

    return instance;
}
