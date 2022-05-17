#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <vector>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

struct VulkanManager
{
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physical_device;
    VkDevice device;
    VkSwapchainKHR swapchain;
    std::vector<uint32_t> queue_family_indices;
    std::vector<VkQueue> queues;
    VkFormat swapchain_format;
    VkExtent2D swapchain_extent;
    std::vector<VkImage> swapchain_images;
    std::vector<VkImageView> swapchain_image_views;

    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
};

enum
{
    DEVICE_EXT_SWAPCHAIN = 0,
    DEVICE_EXT_SYNC_2    = 1,
    DEVICE_EXT_COUNT     = 2
};

struct VulkanInitParams
{
    GLFWwindow* window;
    uint32_t window_width;
    uint32_t window_height;

    std::vector<const char *> instance_extensions;
    std::vector<const char *> instance_layers;
    std::vector<uint32_t> device_extension_ids;

    std::vector<VkQueueFlagBits> queue_flags;

    uint32_t swapchain_image_count;
    VkFormat swapchain_format;
    VkPresentModeKHR swapchain_present_mode;
};

VulkanManager vulkan_init(const VulkanInitParams& params);

void vulkan_release(VulkanManager& vulkan_manager);

VkShaderModule create_shader_module(VkDevice device, const char *filename);

VkCommandPool create_command_pool(VkDevice device, uint32_t q_family_idx);

VkCommandBuffer create_command_buffer(VkDevice device, VkCommandPool pool);

VkFence create_fence(VkDevice device, bool signaled);

VkSemaphore create_semaphore(VkDevice device);

VkBuffer create_buffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage);

VkDeviceMemory allocate_buffer_memory(VkDevice device, VkBuffer buffer, VkMemoryPropertyFlags memory_property_flags, const VkPhysicalDeviceMemoryProperties &physical_device_memory_properties);

void upload_data(VkDevice device, VkCommandPool command_pool, VkQueue queue, VkCommandBuffer command_buffer, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceMemory src_memory, VkDeviceMemory dst_memory, VkDeviceSize size, void* data);

#endif // HELPERS_HPP