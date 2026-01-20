#include "queue.hpp"

std::optional<QueueFamily> get_queue_family(VkPhysicalDevice physical_device, VkSurfaceKHR surface) {
    uint32_t queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, nullptr);

    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_count, queue_families.data());

    std::optional<uint32_t> graphics;
    std::optional<uint32_t> presentation;

    for(uint32_t i = 0; i < queue_families.size(); i++) {
        bool graphics_supported = queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT;
        VkBool32 presentation_supported = VK_FALSE;
        vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &presentation_supported);

        if(graphics_supported && presentation_supported) {
            return QueueFamily {
                .graphics = i,
                .presentation = i
            };
        }

        if(!graphics && graphics_supported) {
            graphics = i;
        }

        if(!presentation && presentation_supported) {
            presentation = i;
        }
    }

    if(graphics.has_value() && presentation.has_value()) {
        return QueueFamily {
            .graphics = graphics.value(),
            .presentation = presentation.value()
        };
    }

    return std::nullopt;
}
