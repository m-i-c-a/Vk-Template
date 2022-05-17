#include <array>
#include <string.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

// #include <imgui/imgui.h>
// #include <imgui/backends/imgui_impl_glfw.h>
// #include <imgui/backends/imgui_impl_opengl3.h>

#include "Defines.hpp"
#include "Helpers.hpp"

enum
{
    QUEUE_GRAPHICS = 0,
    QUEUE_COUNT
};

enum
{
    RENDERPASS_DEFAULT = 0,
    RENDERPASS_COUNT
};

enum
{
    PIPELINE_DEFAULT = 0,
    PIPELINE_COUNT
};

enum
{
    COMMAND_POOL_DEFAULT = 0,
    COMMAND_POOL_COUNT
};

enum
{
    COMMAND_BUFFER_RENDER = 0,
    COMMAND_BUFFER_COUNT
};

enum
{
    SEMAPHORE_COUNT
};

enum
{
    FENCE_IMAGE_ACQUIRE = 0,
    FENCE_COUNT
};

enum
{
    BUFFER_VERTEX_TRIANGLE = 0,
    BUFFER_INDEX_TRIANGLE  = 1,
    BUFFER_STAGING         = 2,
    BUFFER_COUNT
};

VulkanManager g_vk;

struct VulkanApp
{
    VkRenderPass renderpass[RENDERPASS_COUNT];
    std::vector<VkFramebuffer> framebuffers;

    VkPipeline pipeline[PIPELINE_COUNT];
    VkPipelineLayout pipeline_layout[PIPELINE_COUNT];

    VkCommandPool command_pool[COMMAND_POOL_COUNT];
    VkCommandBuffer command_buffer[COMMAND_BUFFER_COUNT];

    VkSemaphore semaphore[SEMAPHORE_COUNT];
    VkFence fence[FENCE_COUNT];

    VkBuffer buffer[BUFFER_COUNT];
    VkDeviceMemory buffer_memory[BUFFER_COUNT];
    uint32_t index_count[BUFFER_COUNT];

    uint32_t current_swapchain_image_idx = 0u;
} g_vk_app;

struct AppManager
{
    GLFWwindow *window;
    uint32_t window_width = 500;
    uint32_t window_height = 500;
} g_app;

void init()
{
    const VulkanInitParams vk_init_params{
        .window = g_app.window,
        .window_width = g_app.window_width,
        .window_height = g_app.window_height,
        .instance_extensions = {"VK_KHR_surface", "VK_KHR_xcb_surface"},
        .instance_layers = {"VK_LAYER_KHRONOS_validation"},
        .device_extension_ids = {DEVICE_EXT_SWAPCHAIN, DEVICE_EXT_SYNC_2},
        .queue_flags = {VK_QUEUE_GRAPHICS_BIT},
        .swapchain_image_count = 2u,
        .swapchain_format = VK_FORMAT_R8G8B8A8_SRGB,
        .swapchain_present_mode = VK_PRESENT_MODE_FIFO_KHR};

    g_vk = vulkan_init(vk_init_params);

    // create renderpasses
    {
        const VkAttachmentDescription attachments[1]{
            {
                // Color
                .format = g_vk.swapchain_format,
                .samples = VK_SAMPLE_COUNT_1_BIT,
                .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            }};

        const VkAttachmentReference color_reference{
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

        const VkSubpassDescription subpass{
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .colorAttachmentCount = 1,
            .pColorAttachments = &color_reference,
            .pResolveAttachments = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr};

        const VkSubpassDependency dependencies[2]{
            {// First dependency at the start of the renderpass
             // Does the transition from final to initial layout
             .srcSubpass = VK_SUBPASS_EXTERNAL,                             // Producer of the dependency
             .dstSubpass = 0,                                               // Consumer is our single subpass that will wait for the execution dependency
             .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // Match our pWaitDstStageMask when we vkQueueSubmit
             .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // is a loadOp stage for color attachments
             .srcAccessMask = 0,                                            // semaphore wait already does memory dependency for us
             .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,         // is a loadOp CLEAR access mask for color attachments
             .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT},
            {// Second dependency at the end the renderpass
             // Does the transition from the initial to the final layout
             // Technically this is the same as the implicit subpass dependency, but we are gonna state it explicitly here
             .srcSubpass = 0,                                               // Producer of the dependency is our single subpass
             .dstSubpass = VK_SUBPASS_EXTERNAL,                             // Consumer are all commands outside of the renderpass
             .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, // is a storeOp stage for color attachments
             .dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,          // Do not block any subsequent work
             .srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,         // is a storeOp `STORE` access mask for color attachments
             .dstAccessMask = 0,
             .dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT}};

        const VkRenderPassCreateInfo renderpass_create_info{
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0x0,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .subpassCount = 1,
            .pSubpasses = &subpass,
            .dependencyCount = 2,
            .pDependencies = dependencies};

        VK_CHECK(vkCreateRenderPass(g_vk.device, &renderpass_create_info, nullptr, &g_vk_app.renderpass[RENDERPASS_DEFAULT]));
    }

    // create framebuffers
    {
        VkFramebufferCreateInfo framebuffer_create_info{
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = g_vk_app.renderpass[RENDERPASS_DEFAULT],
            .attachmentCount = 1,
            .width = g_vk.swapchain_extent.width,
            .height = g_vk.swapchain_extent.height,
            .layers = 1};

        g_vk_app.framebuffers.resize(g_vk.swapchain_image_views.size());
        for (size_t i = 0; i < g_vk_app.framebuffers.size(); ++i)
        {
            VkImageView attachments[1] = {
                g_vk.swapchain_image_views[i]};

            framebuffer_create_info.pAttachments = attachments;

            VK_CHECK(vkCreateFramebuffer(g_vk.device, &framebuffer_create_info, nullptr, &g_vk_app.framebuffers[i]));
        }
    }

    // create pipeline layouts
    {
        const VkPipelineLayoutCreateInfo pipeline_layout_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = 0,
            .pSetLayouts = nullptr,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges = nullptr,
        };

        VK_CHECK(vkCreatePipelineLayout(g_vk.device, &pipeline_layout_create_info, nullptr, &g_vk_app.pipeline_layout[PIPELINE_DEFAULT]));
    }

    // create pipelines
    {
        const std::array<VkPipelineShaderStageCreateInfo, 2> shader_stage_create_info{{{
                                                                                           .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                                                                           .stage = VK_SHADER_STAGE_VERTEX_BIT,
                                                                                           .module = create_shader_module(g_vk.device, "../shaders/default-vert.spv"),
                                                                                           .pName = "main",
                                                                                       },
                                                                                       {
                                                                                           .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                                                                                           .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                                                                                           .module = create_shader_module(g_vk.device, "../shaders/default-frag.spv"),
                                                                                           .pName = "main",
                                                                                       }}};

        const std::array<VkVertexInputBindingDescription, 1> vertex_input_binding_description{{{.binding = 0,
                                                                                                .stride = sizeof(float) * 3,
                                                                                                .inputRate = VK_VERTEX_INPUT_RATE_VERTEX}}};

        const std::array<VkVertexInputAttributeDescription, 1> vertex_input_attribute_description{{
            {.location = 0,
             .binding = 0,
             .format = VK_FORMAT_R32G32B32_SFLOAT,
             .offset = 0},
        }};

        const VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = static_cast<uint32_t>(vertex_input_binding_description.size()),
            .pVertexBindingDescriptions = vertex_input_binding_description.data(),
            .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertex_input_attribute_description.size()),
            .pVertexAttributeDescriptions = vertex_input_attribute_description.data()};

        const VkViewport viewport{
            .x = 0,
            .y = 0,
            .width = static_cast<float>(g_vk.swapchain_extent.width),
            .height = static_cast<float>(g_vk.swapchain_extent.height),
            .minDepth = 0.0f,
            .maxDepth = 1.0f,
        };

        const VkRect2D scissor_rect{
            .offset = {.x = 0, .y = 0},
            .extent = g_vk.swapchain_extent};

        const VkPipelineViewportStateCreateInfo viewport_state_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = 1,
            .pViewports = &viewport,
            .scissorCount = 1,
            .pScissors = &scissor_rect,
        };

        const VkPipelineColorBlendAttachmentState blend_attachment_state{
            .blendEnable = VK_FALSE,
            .srcColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            .colorBlendOp = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        };

        const VkPipelineColorBlendStateCreateInfo color_blend_state_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &blend_attachment_state,
            .blendConstants = {0, 0, 0, 0}};

        const VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
            .primitiveRestartEnable = VK_FALSE,
        };

        const VkPipelineRasterizationStateCreateInfo rasterization_state_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = VK_POLYGON_MODE_FILL,
            .cullMode = VK_CULL_MODE_NONE,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp = 0.0f,
            .depthBiasSlopeFactor = 0.0f,
            .lineWidth = 1.0f,
        };

        const VkPipelineMultisampleStateCreateInfo multisample_state_create_info{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .sampleShadingEnable = VK_FALSE,
            .minSampleShading = 0.0f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable = VK_FALSE,
        };

        const VkGraphicsPipelineCreateInfo pipeline_create_info{
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = static_cast<uint32_t>(shader_stage_create_info.size()),
            .pStages = shader_stage_create_info.data(),
            .pVertexInputState = &vertex_input_state_create_info,
            .pInputAssemblyState = &input_assembly_state_create_info,
            .pViewportState = &viewport_state_create_info,
            .pRasterizationState = &rasterization_state_create_info,
            .pMultisampleState = &multisample_state_create_info,
            .pColorBlendState = &color_blend_state_create_info,
            .layout = g_vk_app.pipeline_layout[PIPELINE_DEFAULT],
            .renderPass = g_vk_app.renderpass[RENDERPASS_DEFAULT],
            .subpass = 0,
            .basePipelineHandle = VK_NULL_HANDLE,
            .basePipelineIndex = 0,
        };

        VK_CHECK(vkCreateGraphicsPipelines(g_vk.device, VK_NULL_HANDLE, 1, &pipeline_create_info, nullptr, &g_vk_app.pipeline[PIPELINE_DEFAULT]));

        vkDestroyShaderModule(g_vk.device, shader_stage_create_info[0].module, nullptr);
        vkDestroyShaderModule(g_vk.device, shader_stage_create_info[1].module, nullptr);
    }

    // Command Pools / Buffers
    {
        g_vk_app.command_pool[COMMAND_POOL_DEFAULT] = create_command_pool(g_vk.device, g_vk.queue_family_indices[QUEUE_GRAPHICS]);
        g_vk_app.command_buffer[COMMAND_BUFFER_RENDER] = create_command_buffer(g_vk.device, g_vk_app.command_pool[COMMAND_POOL_DEFAULT]);
    }

    // Fences / Semaphores
    {
        g_vk_app.fence[FENCE_IMAGE_ACQUIRE] = create_fence(g_vk.device, false);
    }

    // create scene
    {
        std::array<float, 9> vertices {
           -0.5f,  0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
            0.0f, -0.5f, 0.0f
        };

        std::array<uint32_t, 3> indices {
            0, 1, 2
        };

        const VkDeviceSize vertex_buffer_size = sizeof(float) * vertices.size();
        const VkDeviceSize index_buffer_size = sizeof(uint32_t) * indices.size();
        const VkDeviceSize staging_buffer_size = std::max(vertex_buffer_size, index_buffer_size);

        g_vk_app.buffer[BUFFER_VERTEX_TRIANGLE] = create_buffer(g_vk.device, vertex_buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
        g_vk_app.buffer_memory[BUFFER_VERTEX_TRIANGLE] = allocate_buffer_memory(g_vk.device, g_vk_app.buffer[BUFFER_VERTEX_TRIANGLE], VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, g_vk.physical_device_memory_properties);

        g_vk_app.buffer[BUFFER_INDEX_TRIANGLE] = create_buffer(g_vk.device, index_buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT);
        g_vk_app.buffer_memory[BUFFER_INDEX_TRIANGLE] = allocate_buffer_memory(g_vk.device, g_vk_app.buffer[BUFFER_INDEX_TRIANGLE], VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, g_vk.physical_device_memory_properties);

        g_vk_app.buffer[BUFFER_STAGING] = create_buffer(g_vk.device, staging_buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
        g_vk_app.buffer_memory[BUFFER_STAGING] = allocate_buffer_memory(g_vk.device, g_vk_app.buffer[BUFFER_STAGING], VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, g_vk.physical_device_memory_properties);

        VK_CHECK(vkBindBufferMemory(g_vk.device, g_vk_app.buffer[BUFFER_VERTEX_TRIANGLE], g_vk_app.buffer_memory[BUFFER_VERTEX_TRIANGLE], 0));
        VK_CHECK(vkBindBufferMemory(g_vk.device, g_vk_app.buffer[BUFFER_INDEX_TRIANGLE], g_vk_app.buffer_memory[BUFFER_INDEX_TRIANGLE], 0));
        VK_CHECK(vkBindBufferMemory(g_vk.device, g_vk_app.buffer[BUFFER_STAGING], g_vk_app.buffer_memory[BUFFER_STAGING], 0));

        upload_data(g_vk.device, g_vk_app.command_pool[COMMAND_POOL_DEFAULT], g_vk.queues[QUEUE_GRAPHICS], g_vk_app.command_buffer[COMMAND_BUFFER_RENDER], 
                    g_vk_app.buffer[BUFFER_STAGING], g_vk_app.buffer[BUFFER_VERTEX_TRIANGLE],
                    g_vk_app.buffer_memory[BUFFER_STAGING], g_vk_app.buffer_memory[BUFFER_VERTEX_TRIANGLE], vertex_buffer_size, vertices.data() );

        upload_data(g_vk.device, g_vk_app.command_pool[COMMAND_POOL_DEFAULT], g_vk.queues[QUEUE_GRAPHICS], g_vk_app.command_buffer[COMMAND_BUFFER_RENDER], 
                    g_vk_app.buffer[BUFFER_STAGING], g_vk_app.buffer[BUFFER_INDEX_TRIANGLE],
                    g_vk_app.buffer_memory[BUFFER_STAGING], g_vk_app.buffer_memory[BUFFER_INDEX_TRIANGLE], index_buffer_size, indices.data() );

        g_vk_app.index_count[BUFFER_VERTEX_TRIANGLE] = indices.size();
    }
}

void render()
{
    VK_CHECK(vkAcquireNextImageKHR(g_vk.device, g_vk.swapchain, UINT64_MAX, VK_NULL_HANDLE, g_vk_app.fence[FENCE_IMAGE_ACQUIRE], &g_vk_app.current_swapchain_image_idx));
    VK_CHECK(vkWaitForFences(g_vk.device, 1, &g_vk_app.fence[FENCE_IMAGE_ACQUIRE], VK_TRUE, UINT64_MAX));
    VK_CHECK(vkResetFences(g_vk.device, 1, &g_vk_app.fence[FENCE_IMAGE_ACQUIRE]));

    static const VkCommandBufferBeginInfo command_buffer_begin_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT};

    static const VkClearValue clear_value{
        .color = {0.22f, 0.22f, 0.22f, 1.0f}};

    VkRenderPassBeginInfo renderpass_begin_info{
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = g_vk_app.renderpass[RENDERPASS_DEFAULT],
        .framebuffer = g_vk_app.framebuffers[g_vk_app.current_swapchain_image_idx],
        .renderArea = {
            .offset = {.x = 0, .y = 0},
            .extent = g_vk.swapchain_extent},
        .clearValueCount = 1,
        .pClearValues = &clear_value,
    };

    vkResetCommandPool(g_vk.device, g_vk_app.command_pool[COMMAND_POOL_DEFAULT], 0x0);

    VkCommandBuffer cmd_buff = g_vk_app.command_buffer[COMMAND_BUFFER_RENDER];

    VK_CHECK(vkBeginCommandBuffer(cmd_buff, &command_buffer_begin_info));

    vkCmdBeginRenderPass(cmd_buff, &renderpass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(cmd_buff, VK_PIPELINE_BIND_POINT_GRAPHICS, g_vk_app.pipeline[PIPELINE_DEFAULT]);

    VkDeviceSize offsets = 0;
    vkCmdBindVertexBuffers(cmd_buff, 0, 1, &g_vk_app.buffer[BUFFER_VERTEX_TRIANGLE], &offsets);
    vkCmdBindIndexBuffer(cmd_buff, g_vk_app.buffer[BUFFER_INDEX_TRIANGLE], 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(cmd_buff, g_vk_app.index_count[BUFFER_VERTEX_TRIANGLE], 1, 0, 0, 0);

    vkCmdEndRenderPass(cmd_buff);

    VK_CHECK(vkEndCommandBuffer(cmd_buff));

    const VkSubmitInfo submit_info{
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .pWaitDstStageMask = nullptr,
        .commandBufferCount = 1,
        .pCommandBuffers = &cmd_buff,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = nullptr,
    };

    VK_CHECK(vkQueueSubmit(g_vk.queues[QUEUE_GRAPHICS], 1, &submit_info, VK_NULL_HANDLE));

    VK_CHECK(vkDeviceWaitIdle(g_vk.device));

    //*** Present (wait for graphics work to complete)
    const VkPresentInfoKHR present_info{
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = nullptr,
        .swapchainCount = 1,
        .pSwapchains = &g_vk.swapchain,
        .pImageIndices = &g_vk_app.current_swapchain_image_idx,
        .pResults = nullptr,
    };

    VK_CHECK(vkQueuePresentKHR(g_vk.queues[QUEUE_GRAPHICS], &present_info));

    VK_CHECK(vkDeviceWaitIdle(g_vk.device));
}

void gui()
{
}

void release()
{
    for (size_t i = 0; i < BUFFER_COUNT; ++i)
    {
        vkFreeMemory(g_vk.device, g_vk_app.buffer_memory[i], nullptr);
        vkDestroyBuffer(g_vk.device, g_vk_app.buffer[i], nullptr);
    }

    for (size_t i = 0; i < COMMAND_POOL_COUNT; ++i)
        vkDestroyCommandPool(g_vk.device, g_vk_app.command_pool[i], nullptr);

    for (size_t i = 0; i < SEMAPHORE_COUNT; ++i)
        vkDestroySemaphore(g_vk.device, g_vk_app.semaphore[i], nullptr);

    for (size_t i = 0; i < FENCE_COUNT; ++i)
        vkDestroyFence(g_vk.device, g_vk_app.fence[i], nullptr);

    for (size_t i = 0; i < PIPELINE_COUNT; ++i)
    {
        vkDestroyPipelineLayout(g_vk.device, g_vk_app.pipeline_layout[i], nullptr);
        vkDestroyPipeline(g_vk.device, g_vk_app.pipeline[i], nullptr);
    }

    for (size_t i = 0; i < g_vk_app.framebuffers.size(); ++i)
        vkDestroyFramebuffer(g_vk.device, g_vk_app.framebuffers[i], nullptr);

    for (size_t i = 0; i < RENDERPASS_COUNT; ++i)
        vkDestroyRenderPass(g_vk.device, g_vk_app.renderpass[i], nullptr);

    vulkan_release(g_vk);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    g_app.window = glfwCreateWindow(g_app.window_width, g_app.window_height, "Vk-Template", nullptr, nullptr);

    if (g_app.window == nullptr)
    {
        glfwTerminate();
        EXIT("=> Failure <=\n");
    }
    glfwMakeContextCurrent(g_app.window);

    LOG("-- Begin -- Init\n");
    init();
    LOG("-- End -- Init\n");

    LOG("-- Begin -- Run\n");

    while (!glfwWindowShouldClose(g_app.window))
    {
        glfwPollEvents();

        render();

        gui();

        glfwSwapBuffers(g_app.window);
    }

    LOG("-- End -- Run\n");

    release();

    glfwDestroyWindow(g_app.window);
    glfwTerminate();

    LOG("-- Release Successful --\n");

    return 0;
}