#include "device.hpp"

#include "util.hpp"

int score_device(VkPhysicalDevice physical_device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physical_device, &properties);

    int score = 0;
    if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }

    score += properties.limits.maxImageDimension2D;

    return score;
}

VkDevice create_logical_device(VkPhysicalDevice device, QueueFamily queue_family, const std::vector<const char*>& device_extensions) {
    float queue_priority = 1.0f;

    std::vector<uint32_t> unique_families { queue_family.graphics, queue_family.presentation };
    std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
    queue_create_infos.reserve(unique_families.size());

    if(queue_family.graphics == queue_family.presentation) {
        VkDeviceQueueCreateInfo queue_create_info {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = queue_family.graphics,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority
        };
        queue_create_infos.push_back(queue_create_info);
    }
    else {
        for(uint32_t queue_family : unique_families) {
            VkDeviceQueueCreateInfo queue_create_info {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = queue_family,
                .queueCount = 1,
                .pQueuePriorities = &queue_priority
            };
            queue_create_infos.push_back(queue_create_info);
        }
    }

    VkDevice logical_device;
    VkPhysicalDeviceFeatures device_features{};
    VkDeviceCreateInfo create_info {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
        .pQueueCreateInfos = queue_create_infos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
        .ppEnabledExtensionNames = device_extensions.data(),
        .pEnabledFeatures = &device_features
    };

    if(vkCreateDevice(device, &create_info, nullptr, &logical_device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device");
    }

    return logical_device;
}

VkPhysicalDevice create_physical_device(VkInstance instance, VkSurfaceKHR surface, const RendererExtensions& extensions) {
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;

    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(instance, &device_count, nullptr);

    if(device_count == 0) {
        throw std::runtime_error("no physical devices found");
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    vkEnumeratePhysicalDevices(instance, &device_count, devices.data());

    std::vector<int> scores;
    for(const auto& device : devices) {
        if(is_gpu_usable(device, surface, extensions)) {
            scores.push_back(score_device(device));
        }
        else {
            scores.push_back(0);
        }
    }

    if(const auto [score, index] = get_max(scores); score != -1) {
        physical_device = devices[index];
    }

    if(physical_device == VK_NULL_HANDLE) {
        throw std::runtime_error("no suitable physical device was found");
    }

    return physical_device;
}
