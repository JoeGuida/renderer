#ifndef API_VULKAN_HPP
#define API_VULKAN_HPP

#if defined(WIN32) || defined(WIN64)

#define VK_USE_PLATFORM_WIN32_KHR
#include <platform/windows/win32_window.hpp>

#endif

#include <expected>
#include <optional>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "command.hpp"
#include "context.hpp"
#include "device.hpp"
#include "extension.hpp"
#include "queue.hpp"
#include "swapchain.hpp"

bool is_gpu_usable(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<const char*> required_extensions);
VkPhysicalDevice get_physical_device(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& device_extensions);
VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats, VkFormat format, VkColorSpaceKHR color_space);
VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes, VkPresentModeKHR mode);
VkInstance create_instance(const std::vector<const char*> validation_layers, const std::vector<const char*> instance_extensions);
bool validation_layers_available(const std::vector<const char*>& validation_layers);
VkDevice create_logical_device(VkPhysicalDevice device, QueueFamily queue_family, const std::vector<const char*>& device_extensions);
VkSurfaceKHR create_window_surface(VkInstance vk_instance, HWND hwnd, HINSTANCE instance);
RenderQueue get_queues(VkDevice logical_device, uint32_t graphics_queuee_id, uint32_t presentation_queue_id);
void create_image_views(VkDevice device, Swapchain& swapchain);
VkRenderPass create_render_pass(VkDevice device, VkFormat format);
std::pair<VkPipelineLayout, VkPipeline> create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass render_pass);
void create_framebuffers(VkDevice device, Swapchain& swapchain, VkRenderPass render_pass);
void record_command_buffer(Swapchain& swapchain, VkCommandBuffer command_buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, VkPipeline graphics_pipeline);
std::pair<std::vector<VkSemaphore>, std::vector<VkFence>> create_sync_objects(VkDevice device);
void draw(VkContext context);
std::expected<VkContext, std::string> init_renderer(PlatformWindow* window, HINSTANCE instance, const RendererExtensions& extensions);

#endif
