#ifndef RENDERER_UTIL_HPP
#define RENDERER_UTIL_HPP

#include <utility>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "extension.hpp"
#include "queue.hpp"
#include "swapchain.hpp"

inline bool is_gpu_usable(PhysicalDevice device, VkSurfaceKHR surface, const RendererExtensions& extensions) {
    if(!get_queue_family(device.handle, surface).has_value()) {
        return false;
    };

    if(!device_extensions_supported(device.handle, extensions)) {
        return false;
    }

    SwapchainSupportInfo swapchain_support = query_swapchain_support(device.handle, surface);

    return !swapchain_support.formats.empty() && !swapchain_support.present_modes.empty();
}

inline std::pair<int, int> get_max(const std::vector<int>& scores) {
    int highest_score = -1;
    int index = 0;
    for(int i = 0; i < scores.size(); i++) {
        if(scores[i] > highest_score) {
            highest_score = scores[i];
            index = i;
        }
    }

    return { highest_score, index };
}

#endif
