#ifndef RENDERER_DEBUG_MESSENGER_HPP
#define RENDERER_DEBUG_MESSENGER_HPP

#include <vulkan/vulkan.hpp>

VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* callback_data, void* user_data);
VkResult create_debug_utils_messenger_ext(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* create_info, const VkAllocationCallbacks* allocator, VkDebugUtilsMessengerEXT* debug_messenger);
void destroy_debug_utils_messenger_ext(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger);
VkDebugUtilsMessengerEXT setup_debug_messenger(VkInstance instance);

#endif
