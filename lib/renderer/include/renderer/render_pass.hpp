#ifndef RENDERER_RENDER_PASS_HPP
#define RENDERER_RENDER_PASS_HPP

#include <vulkan/vulkan.hpp>

VkRenderPass create_render_pass(VkDevice device, VkFormat format);

#endif
