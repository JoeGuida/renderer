#ifndef RENDERER_RENDER_PASS_HPP
#define RENDERER_RENDER_PASS_HPP

#include <vulkan/vulkan.hpp>

#include <renderer/device.hpp>

VkRenderPass create_render_pass(const Device& device, VkFormat format);

#endif
