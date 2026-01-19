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

#include "device.hpp"
#include "queue.hpp"
#include "swapchain.hpp"

struct VkContext {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDevice physical_device;
    VulkanDevice device;
    VkSurfaceKHR surface;
    RenderQueue queue;
    Swapchain swapchain;
    VkRenderPass render_pass;
    VkPipeline pipeline;
    VkPipelineLayout pipeline_layout;
    VkCommandPool command_pool;
    VkCommandBuffer command_buffer;

    std::vector<VkSemaphore> semaphores;
    std::vector<VkFence> fences;
};

std::optional<QueueFamily> get_queue_family(VkPhysicalDevice physical_device, VkSurfaceKHR surface);
int score_device(VkPhysicalDevice physical_device);
bool is_gpu_usable(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<const char*> required_extensions);
VkPhysicalDevice get_physical_device(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char*>& device_extensions);
VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR>& available_formats, VkFormat format, VkColorSpaceKHR color_space);
VkPresentModeKHR choose_present_mode(const std::vector<VkPresentModeKHR>& available_present_modes, VkPresentModeKHR mode);
VkInstance create_instance(const std::vector<const char*> validation_layers, const std::vector<const char*> instance_extensions);
bool validation_layers_available(const std::vector<const char*>& validation_layers);
VkDevice create_logical_device(VkPhysicalDevice device, QueueFamily queue_family, const std::vector<const char*>& device_extensions);
VkSurfaceKHR create_window_surface(VkInstance vk_instance, HWND hwnd, HINSTANCE instance);
RenderQueue get_queues(VkDevice logical_device, uint32_t graphics_queuee_id, uint32_t presentation_queue_id);
std::vector<VkImageView> create_image_views(VkDevice device, const std::vector<VkImage>& swapchain_images, VkFormat image_format);
VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& code);
VkRenderPass create_render_pass(VkDevice device, VkFormat format);
std::pair<VkPipelineLayout, VkPipeline> create_graphics_pipeline(VkDevice device, VkExtent2D extent, VkRenderPass render_pass);
void create_framebuffers(VkDevice device, Swapchain& swapchain, VkRenderPass render_pass);
VkCommandPool create_command_pool(VkDevice device, uint32_t graphics_queue_family);
VkCommandBuffer create_command_buffer(VkDevice device, VkCommandPool command_pool);
void record_command_buffer(Swapchain& swapchain, VkCommandBuffer command_buffer, VkRenderPass render_pass, VkFramebuffer framebuffer, VkPipeline graphics_pipeline);
std::pair<std::vector<VkSemaphore>, std::vector<VkFence>> create_sync_objects(VkDevice device);
void draw(VkContext context);
void cleanup(VkContext context);
std::expected<VkContext, std::string> init_renderer(PlatformWindow* window, HINSTANCE instance, const std::vector<const char*>& validation_layers, const std::vector<const char*>& instance_extensions, const std::vector<const char*>& device_extensions);

#endif
