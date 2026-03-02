#include <renderer/frame_data.hpp>

void destroy(const Device& device, const FrameData& frame_data) {
    vkDestroySemaphore(device.logical, frame_data.begin, nullptr);
    vkDestroySemaphore(device.logical, frame_data.in_flight, nullptr);
    vkDestroyFence(device.logical, frame_data.fence, nullptr);
}
