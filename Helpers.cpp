#include <array>
#include <string.h>

#include "Helpers.hpp"
#include "Defines.hpp"

namespace
{
    VkPhysicalDeviceSynchronization2FeaturesKHR sync_2_features{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES_KHR,
        .synchronization2 = VK_TRUE};
}

static VkInstance create_instance(const std::vector<const char *> &extensions, const std::vector<const char *> &layers)
{
    constexpr VkApplicationInfo app_info{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .apiVersion = VK_API_VERSION_1_2};

    const VkInstanceCreateInfo instance_create_info{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pNext = nullptr,
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<uint32_t>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };

    VkInstance instance;
    VK_CHECK(vkCreateInstance(&instance_create_info, nullptr, &instance));
    return instance;
}

static VkSurfaceKHR create_surface(VkInstance instance, GLFWwindow *window)
{
    VkSurfaceKHR surface;
    VK_CHECK(glfwCreateWindowSurface(instance, window, nullptr, &surface));
    return surface;
}

static VkPhysicalDevice select_physical_device(VkInstance instance)
{
    uint32_t num_physical_devices = 0;
    vkEnumeratePhysicalDevices(instance, &num_physical_devices, nullptr);
    std::vector<VkPhysicalDevice> physical_devices(num_physical_devices);
    vkEnumeratePhysicalDevices(instance, &num_physical_devices, physical_devices.data());

    LOG("# Physical Devices: %u\n", num_physical_devices);
    for (uint32_t i = 0; i < num_physical_devices; ++i)
    {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(physical_devices[i], &props);
        LOG("%i : %s\n", i, props.deviceName);
    }

    const uint32_t physical_device_index = 2u;
    LOG("Using Physical Device %u\n\n", physical_device_index);

    return physical_devices[physical_device_index];
}

static std::vector<uint32_t> select_q_family_indices(VkPhysicalDevice physical_device, VkSurfaceKHR surface, const std::vector<VkQueueFlagBits> &q_flags)
{
    uint32_t num_q_family_props = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_q_family_props, nullptr);
    std::vector<VkQueueFamilyProperties> q_family_props(num_q_family_props);
    vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &num_q_family_props, q_family_props.data());

    auto get_q_family_idx = [&](const VkQueueFlagBits q_flag, const bool present)
    {
        // Dedicated queue for compute
        // Try to find a queue family index that supports compute but not graphics
        if (q_flag & VK_QUEUE_COMPUTE_BIT)
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(q_family_props.size()); i++)
            {
                if ((q_family_props[i].queueFlags & q_flag) && ((q_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
                {
                    return i;
                }
            }
        }

        // Dedicated queue for transfer
        // Try to find a queue family index that supports transfer but not graphics and compute
        if (q_flag & VK_QUEUE_TRANSFER_BIT)
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(q_family_props.size()); i++)
            {
                if ((q_family_props[i].queueFlags & q_flag) && ((q_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((q_family_props[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
                {
                    return i;
                }
            }
        }

        // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
        for (uint32_t i = 0; i < static_cast<uint32_t>(q_family_props.size()); i++)
        {
            if (q_family_props[i].queueFlags & q_flag)
            {
                if (present)
                {
                    VkBool32 q_fam_supports_present = false;
                    vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &q_fam_supports_present);
                    if (q_fam_supports_present)
                        return i;
                }
                else
                {
                    return i;
                }
            }
        }

        EXIT("Could not find a matching queue family index");
    };

    std::vector<uint32_t> q_family_indices(q_flags.size(), 0x0);

    for (uint32_t i = 0; i < q_flags.size(); ++i)
    {
        q_family_indices[i] = get_q_family_idx(q_flags[i], (q_flags[i] & VK_QUEUE_GRAPHICS_BIT) ? true : false);
    }

    return q_family_indices;
}

static VkDevice create_device(VkPhysicalDevice physical_device, const std::vector<uint32_t> &q_family_indices, const std::vector<uint32_t> &device_extension_ids)
{
    const float q_priority = 1.0f;

    std::vector<VkDeviceQueueCreateInfo> q_create_infos;
    q_create_infos.reserve(q_family_indices.size());
    for (const uint32_t idx : q_family_indices)
    {
        const VkDeviceQueueCreateInfo q_create_info{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = idx,
            .queueCount = 1u,
            .pQueuePriorities = &q_priority};

        q_create_infos.push_back(q_create_info);
    }

    std::vector<const char *> device_extensions;
    void *p_next_chain = nullptr;
    void **next_p_next = nullptr;

    for (const uint32_t ext_id : device_extension_ids)
    {
        switch (ext_id)
        {
        case DEVICE_EXT_SYNC_2:
        {
            if (p_next_chain == nullptr)
            {
                p_next_chain = (void *)(&sync_2_features);
                next_p_next = &sync_2_features.pNext;
            }
            else
            {
                *next_p_next = &sync_2_features;
            }
            break;
        }
        case DEVICE_EXT_SWAPCHAIN:
            device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
            break;
        default:
            EXIT("Unsupported device extension specified!");
            break;
        }
    }

    const VkDeviceCreateInfo device_create_info{
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = p_next_chain,
        .queueCreateInfoCount = static_cast<uint32_t>(q_create_infos.size()),
        .pQueueCreateInfos = q_create_infos.data(),
        .enabledLayerCount = 0u,
        .ppEnabledLayerNames = nullptr,
        .enabledExtensionCount = static_cast<uint32_t>(device_extensions.size()),
        .ppEnabledExtensionNames = device_extensions.data(),
        .pEnabledFeatures = nullptr};

    VkDevice device;
    VK_CHECK(vkCreateDevice(physical_device, &device_create_info, nullptr, &device));
    return device;
}

static std::vector<VkQueue> get_queues(VkDevice device, const std::vector<uint32_t> &q_family_indices)
{
    std::vector<VkQueue> queues(q_family_indices.size(), VK_NULL_HANDLE);

    for (uint32_t i = 0; i < q_family_indices.size(); ++i)
    {
        vkGetDeviceQueue(device, q_family_indices[i], 0, &queues[i]);
    }

    return queues;
}

static VkSwapchainCreateInfoKHR populate_swapchain_create_info(VkPhysicalDevice physical_device, VkSurfaceKHR surface, uint32_t image_count, VkFormat format, VkExtent2D extent, VkPresentModeKHR present_mode)
{
    VkSurfaceCapabilitiesKHR surface_capabilities;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &surface_capabilities));
    VkSwapchainCreateInfoKHR swapchain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    swapchain_create_info.surface = surface;

    //** Image Count
    {
        assert(image_count > 0 && "Invalid requested image count for swapchain!");

        // If the minImageCount is 0, then there is not a limit on the number of images the swapchain
        // can support (ignoring memory constraints). See the Vulkan Spec for more information.
        if (surface_capabilities.maxImageCount == 0)
        {
            if (image_count >= surface_capabilities.minImageCount)
            {
                swapchain_create_info.minImageCount = image_count;
            }
            else
            {
                LOG("Failed to create Swapchain. The requested number of images %u does not meet the minimum requirement of %u\n", image_count, surface_capabilities.minImageCount);
                exit(EXIT_FAILURE);
            }
        }
        else if (image_count >= surface_capabilities.minImageCount &&
                 image_count <= surface_capabilities.maxImageCount)
        {
            swapchain_create_info.minImageCount = image_count;
        }
        else
        {
            LOG("The number of requested Swapchain images %u is not supported. Min: %u Max: %u\n", image_count, surface_capabilities.minImageCount, surface_capabilities.maxImageCount);
            exit(EXIT_FAILURE);
        }
    }

    //** Image Format
    {
        uint32_t num_supported_surface_formats = 0;
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &num_supported_surface_formats, nullptr));
        std::vector<VkSurfaceFormatKHR> supported_surface_formats(num_supported_surface_formats);
        VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &num_supported_surface_formats, supported_surface_formats.data()));

        bool requested_format_found = false;
        for (uint32_t i = 0; i < num_supported_surface_formats; ++i)
        {
            if (supported_surface_formats[i].format == format)
            {
                swapchain_create_info.imageFormat = supported_surface_formats[i].format;
                swapchain_create_info.imageColorSpace = supported_surface_formats[i].colorSpace;
                requested_format_found = true;
                break;
            }
        }

        if (!requested_format_found)
        {
            LOG("Requested swapchain format not found! Using first one avaliable!\n\n");
            swapchain_create_info.imageFormat = supported_surface_formats[0].format;
            swapchain_create_info.imageColorSpace = supported_surface_formats[0].colorSpace;
        }
    }

    //** Extent
    {
        // The Vulkan Spec states that if the current width/height is 0xFFFFFFFF, then the surface size
        // will be deteremined by the extent specified in the VkSwapchainCreateInfoKHR.
        if (surface_capabilities.currentExtent.width != (uint32_t)-1)
        {
            swapchain_create_info.imageExtent = extent;
        }
        else
        {
            swapchain_create_info.imageExtent = surface_capabilities.currentExtent;
        }
    }

    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = 0;
    swapchain_create_info.pQueueFamilyIndices = nullptr;

    //** Pre Transform
    {
        if (surface_capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        {
            swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        }
        else
        {
            swapchain_create_info.preTransform = surface_capabilities.currentTransform;
            LOG("WARNING - Swapchain pretransform is not IDENTITIY_BIT_KHR!\n");
        }
    }

    //** Composite Alpha
    {
        // Determine the composite alpha format the application needs.
        // Find a supported composite alpha format (not all devices support alpha opaque),
        // but we prefer it.
        // Simply select the first composite alpha format available
        // Used for blending with other windows in the system
        VkCompositeAlphaFlagBitsKHR composite_alpha_flags[4] = {
            VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
            VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        };
        for (size_t i = 0; i < 4; ++i)
        {
            if (surface_capabilities.supportedCompositeAlpha & composite_alpha_flags[i])
            {
                swapchain_create_info.compositeAlpha = composite_alpha_flags[i];
                break;
            };
        }
    }

    //** Present Mode
    {
        uint32_t num_supported_present_modes = 0;
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &num_supported_present_modes, nullptr));
        std::vector<VkPresentModeKHR> supported_present_modes(num_supported_present_modes);
        VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &num_supported_present_modes, supported_present_modes.data()));

        // Determine the present mode the application needs.
        // Try to use mailbox, it is the lowest latency non-tearing present mode
        // All devices support FIFO (this mode waits for the vertical blank or v-sync)
        swapchain_create_info.presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (uint32_t i = 0; i < num_supported_present_modes; ++i)
        {
            if (supported_present_modes[i] == present_mode)
            {
                swapchain_create_info.presentMode = present_mode;
                break;
            }
        }
    }

    //** Creating Swapchain
    swapchain_create_info.clipped = VK_TRUE;
    swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

    return swapchain_create_info;
}

static std::vector<VkImage> get_swapchain_images(VkDevice device, VkSwapchainKHR swapchain)
{
    uint32_t num_swapchain_images = 0;
    VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &num_swapchain_images, nullptr));
    std::vector<VkImage> images(num_swapchain_images, VK_NULL_HANDLE);
    VK_CHECK(vkGetSwapchainImagesKHR(device, swapchain, &num_swapchain_images, images.data()));

    LOG("Swapchain Image Count: %u\n", num_swapchain_images);
    return images;
}

static std::vector<VkImageView> create_swapchain_image_views(VkDevice device, const std::vector<VkImage> &images, const VkFormat format)
{
    VkImageViewCreateInfo image_view_create_info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY},
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = 0,
            .layerCount = 1,
        }};

    std::vector<VkImageView> image_views(images.size(), VK_NULL_HANDLE);

    for (size_t i = 0; i < images.size(); ++i)
    {
        image_view_create_info.image = images[i];
        VK_CHECK(vkCreateImageView(device, &image_view_create_info, nullptr, &image_views[i]));
    }

    return image_views;
}

static uint32_t get_heap_idx(const uint32_t memory_type_indices, const VkMemoryPropertyFlags memory_property_flags, VkPhysicalDeviceMemoryProperties memory_properties)
{
    // Iterate over all memory types available for the device used in this example
    for (uint32_t i = 0; i < memory_properties.memoryTypeCount; i++)
    {
        if (memory_type_indices & (1 << i) && (memory_properties.memoryTypes[i].propertyFlags & memory_property_flags) == memory_property_flags)
        {
            return i;
        }
    }

    assert(false && "Could not find suitable memory type!");
    return 0;
}


VulkanManager vulkan_init(const VulkanInitParams& params)
{
    VkInstance instance = create_instance(params.instance_extensions, params.instance_layers);
    VkSurfaceKHR surface = create_surface(instance, params.window);
    VkPhysicalDevice physical_device = select_physical_device(instance);
    std::vector<uint32_t> q_family_indices = select_q_family_indices(physical_device, surface, params.queue_flags);
    VkDevice device = create_device(physical_device, q_family_indices, params.device_extension_ids);
    std::vector<VkQueue> queues = get_queues(device, q_family_indices);

    VkSwapchainCreateInfoKHR swapchain_create_info = populate_swapchain_create_info(physical_device, surface, params.swapchain_image_count, params.swapchain_format, {params.window_width, params.window_height}, params.swapchain_present_mode);
    VkFormat swapchain_format = swapchain_create_info.imageFormat;
    VkExtent2D swapchain_extent = swapchain_create_info.imageExtent;

    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    VK_CHECK(vkCreateSwapchainKHR(device, &swapchain_create_info, nullptr, &swapchain));
    std::vector<VkImage> swapchain_images = get_swapchain_images(device, swapchain);
    std::vector<VkImageView> swapchain_image_views = create_swapchain_image_views(device, swapchain_images, swapchain_format);

    VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
    vkGetPhysicalDeviceMemoryProperties(physical_device, &physical_device_memory_properties);

    VulkanManager vulkan_manager {
        .instance = instance,
        .surface = surface,
        .physical_device = physical_device,
        .device = device,
        .swapchain = swapchain,
        .queue_family_indices = q_family_indices,
        .queues = queues,
        .swapchain_format = swapchain_format,
        .swapchain_extent = swapchain_extent,
        .swapchain_images = swapchain_images,
        .swapchain_image_views = swapchain_image_views,
        .physical_device_memory_properties = physical_device_memory_properties
    };

    return vulkan_manager;
}

void vulkan_release(VulkanManager& vulkan_manager)
{
    for (uint32_t i = 0; i < vulkan_manager.swapchain_images.size(); ++i)
        vkDestroyImageView(vulkan_manager.device, vulkan_manager.swapchain_image_views[i], nullptr);

    vkDestroySwapchainKHR(vulkan_manager.device, vulkan_manager.swapchain, nullptr);
    vkDestroyDevice(vulkan_manager.device, nullptr);
    vkDestroySurfaceKHR(vulkan_manager.instance, vulkan_manager.surface, nullptr);
    vkDestroyInstance(vulkan_manager.instance, nullptr);
}

VkShaderModule create_shader_module(VkDevice device, const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Failed to open file %s!\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_END);
    const size_t nbytes_file_size = (size_t)ftell(f);
    rewind(f);

    uint32_t *buffer = (uint32_t *)malloc(nbytes_file_size);
    fread(buffer, nbytes_file_size, 1, f);
    fclose(f);

    const VkShaderModuleCreateInfo ci_shader_module{
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .codeSize = nbytes_file_size,
        .pCode = buffer,
    };

    VkShaderModule vk_shader_module;
    VkResult result = vkCreateShaderModule(device, &ci_shader_module, NULL, &vk_shader_module);
    if (result != VK_SUCCESS)
    {
        printf("Failed to create shader module for %s!\n", filename);
        exit(EXIT_FAILURE);
    }

    free(buffer);

    return vk_shader_module;
}

/**
 * @brief Create a Command Pool object
 * 
 * @param device 
 * @param graphicsQueueFamilyIndex 
 * @return VkCommandPool 
 * 
 * CommandPools should be created without RESET_COMMAND_BUFFER_BIT and be reset manually via vkResetCommandPool
 *  for performance. See (https://arm-software.github.io/vulkan_best_practice_for_mobile_developers/samples/performance/command_buffer_usage/command_buffer_usage_tutorial.html)
 */
VkCommandPool create_command_pool(VkDevice device, uint32_t q_family_idx)
{
    const VkCommandPoolCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0x0,
        .queueFamilyIndex = q_family_idx};

    VkCommandPool pool;
    VK_CHECK(vkCreateCommandPool(device, &create_info, nullptr, &pool));
    return pool;
}


VkCommandBuffer create_command_buffer(VkDevice device, VkCommandPool pool)
{
    const VkCommandBufferAllocateInfo commandBufferAllocateInfo{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer command_buffer;
    VK_CHECK(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, &command_buffer));
    return command_buffer;
}

VkFence create_fence(VkDevice device, bool signaled)
{
    const VkFenceCreateInfo fenceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = (signaled) ? VK_FENCE_CREATE_SIGNALED_BIT : 0u,
    };

    VkFence fence;
    VK_CHECK(vkCreateFence(device, &fenceCreateInfo, nullptr, &fence));
    return fence;
}

VkSemaphore create_semaphore(VkDevice device)
{
    VkSemaphoreCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    VkSemaphore semaphore;
    VK_CHECK(vkCreateSemaphore(device, &createInfo, nullptr, &semaphore));
    return semaphore;
}


VkBuffer create_buffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage)
{
    const VkBufferCreateInfo create_info{
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VkBuffer buffer;
    VK_CHECK(vkCreateBuffer(device, &create_info, nullptr, &buffer));
    return buffer;
}

VkDeviceMemory allocate_buffer_memory(VkDevice device, VkBuffer buffer, VkMemoryPropertyFlags memory_property_flags, const VkPhysicalDeviceMemoryProperties &physical_device_memory_properties)
{
    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(device, buffer, &memReqs);

    const VkMemoryAllocateInfo allocInfo{
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memReqs.size,
        .memoryTypeIndex = get_heap_idx(memReqs.memoryTypeBits, memory_property_flags, physical_device_memory_properties)};

    VkDeviceMemory memory;
    VK_CHECK(vkAllocateMemory(device, &allocInfo, nullptr, &memory));
    return memory;
}

void upload_data(VkDevice device, VkCommandPool command_pool, VkQueue queue, VkCommandBuffer command_buffer, VkBuffer src_buffer, VkBuffer dst_buffer, VkDeviceMemory src_memory, VkDeviceMemory dst_memory, VkDeviceSize size, void* data)
{
    void *staging_data;
    vkMapMemory(device, src_memory, 0, VK_WHOLE_SIZE, 0, &staging_data);
    memcpy(staging_data, data, size);

    VkMappedMemoryRange range{
        .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
        .memory = src_memory,
        .offset = 0,
        .size = VK_WHOLE_SIZE,
    };

    vkFlushMappedMemoryRanges(device, 1, &range);
    vkUnmapMemory(device, src_memory);

    static const VkCommandBufferBeginInfo command_buffer_begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };

    vkBeginCommandBuffer(command_buffer, &command_buffer_begin_info);

    const VkBufferCopy buffer_copy {
        .srcOffset = 0,
        .dstOffset = 0,
        .size = size
    };

    vkCmdCopyBuffer(command_buffer, src_buffer, dst_buffer, 1u, &buffer_copy);

    vkEndCommandBuffer(command_buffer);

    const VkSubmitInfo submit_info {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1u,
        .pCommandBuffers = &command_buffer
    };

    VK_CHECK(vkQueueSubmit(queue, 1u, &submit_info, VK_NULL_HANDLE));

    vkQueueWaitIdle(queue);
    vkResetCommandPool(device, command_pool,  0x0);
}