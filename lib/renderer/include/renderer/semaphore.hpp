#ifndef RENDERER_SEMAPHORE_HPP
#define RENDERER_SEMAPHORE_HPP

#ifdef VULKAN

#include <vulkan/vulkan.hpp>

#include <renderer/device.hpp>

VkSemaphore create_semaphore(const Device& device);

#endif

#endif
