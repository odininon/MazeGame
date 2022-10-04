//
// Created by Michael Stengel on 10/3/2022.
//

#include "vk_engine.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <VkBootstrap.h>
#include <vk_initializers.h>
#include <vk_types.h>

#include <fstream>
#include <iostream>

void VulkanEngine::init() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  _window = glfwCreateWindow(_windowExtent.width, _windowExtent.height, "Vulkan", nullptr, nullptr);

  init_vulkan();
  init_swapchain();
  init_commands();

  init_default_renderpass();

  init_framebuffers();

  init_sync_structures();

  init_pipelines();
}

void VulkanEngine::cleanup() {
  vkDestroySwapchainKHR(_device, _swapchain, nullptr);

  // destroy the main renderpass
  vkDestroyRenderPass(_device, _renderPass, nullptr);

  // destroy swapchain resources
  for (int i = 0; i < _framebuffers.size(); i++) {
    vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);

    vkDestroyImageView(_device, _swapchainImageViews[i], nullptr);
  }

  vkDestroyCommandPool(_device, _commandPool, nullptr);

  vkDestroySwapchainKHR(_device, _swapchain, nullptr);

  for (auto &_swapchainImageView : _swapchainImageViews) {
    vkDestroyImageView(_device, _swapchainImageView, nullptr);
  }

  vkDestroyDevice(_device, nullptr);
  vkDestroySurfaceKHR(_instance, _surface, nullptr);
  vkb::destroy_debug_utils_messenger(_instance, _debug_messenger);
  vkDestroyInstance(_instance, nullptr);

  glfwDestroyWindow(_window);
  glfwTerminate();
}

void VulkanEngine::draw() {
  // wait until the GPU has finished rendering the last frame. Timeout of 1 second
  vkWaitForFences(_device, 1, &_renderFence, true, 1000000000);
  vkResetFences(_device, 1, &_renderFence);

  // request image from the swapchain, one second timeout
  uint32_t swapchainImageIndex;
  vkAcquireNextImageKHR(_device, _swapchain, 1000000000, _presentSemaphore, nullptr, &swapchainImageIndex);

  // now that we are sure that the commands finished executing, we can safely reset the command buffer to begin
  // recording again.
  vkResetCommandBuffer(_mainCommandBuffer, 0);

  // naming it cmd for shorter writing
  VkCommandBuffer cmd = _mainCommandBuffer;

  // begin the command buffer recording. We will use this command buffer exactly once, so we want to let Vulkan know
  // that
  VkCommandBufferBeginInfo cmdBeginInfo = {};
  cmdBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  cmdBeginInfo.pNext = nullptr;

  cmdBeginInfo.pInheritanceInfo = nullptr;
  cmdBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(cmd, &cmdBeginInfo);

  // make a clear-color from frame number. This will flash with a 120*pi frame period.
  VkClearValue clearValue;
  float flash = abs(sin(_frameNumber / 120.f));
  clearValue.color = {{0.0f, 0.0f, flash, 1.0f}};

  // start the main renderpass.
  // We will use the clear color from above, and the framebuffer of the index the swapchain gave us
  VkRenderPassBeginInfo rpInfo = {};
  rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rpInfo.pNext = nullptr;

  rpInfo.renderPass = _renderPass;
  rpInfo.renderArea.offset.x = 0;
  rpInfo.renderArea.offset.y = 0;
  rpInfo.renderArea.extent = _windowExtent;
  rpInfo.framebuffer = _framebuffers[swapchainImageIndex];

  // connect clear values
  rpInfo.clearValueCount = 1;
  rpInfo.pClearValues = &clearValue;

  vkCmdBeginRenderPass(cmd, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);

  vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, _trianglePipeline);
  vkCmdDraw(cmd, 3, 1, 0, 0);

  // finalize the render pass
  vkCmdEndRenderPass(cmd);
  // finalize the command buffer (we can no longer add commands, but it can now be executed)
  vkEndCommandBuffer(cmd);

  // prepare the submission to the queue.
  // we want to wait on the _presentSemaphore, as that semaphore is signaled when the swapchain is ready
  // we will signal the _renderSemaphore, to signal that rendering has finished

  VkSubmitInfo submit = {};
  submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit.pNext = nullptr;

  VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  submit.pWaitDstStageMask = &waitStage;

  submit.waitSemaphoreCount = 1;
  submit.pWaitSemaphores = &_presentSemaphore;

  submit.signalSemaphoreCount = 1;
  submit.pSignalSemaphores = &_renderSemaphore;

  submit.commandBufferCount = 1;
  submit.pCommandBuffers = &cmd;

  // submit command buffer to the queue and execute it.
  //  _renderFence will now block until the graphic commands finish execution
  vkQueueSubmit(_graphicsQueue, 1, &submit, _renderFence);

  // this will put the image we just rendered into the visible window.
  // we want to wait on the _renderSemaphore for that,
  // as it's necessary that drawing commands have finished before the image is displayed to the user
  VkPresentInfoKHR presentInfo = {};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.pNext = nullptr;

  presentInfo.pSwapchains = &_swapchain;
  presentInfo.swapchainCount = 1;

  presentInfo.pWaitSemaphores = &_renderSemaphore;
  presentInfo.waitSemaphoreCount = 1;

  presentInfo.pImageIndices = &swapchainImageIndex;

  vkQueuePresentKHR(_graphicsQueue, &presentInfo);

  // increase the number of frames drawn
  _frameNumber++;
}

void VulkanEngine::run() {
  while (!glfwWindowShouldClose(_window)) {
    glfwPollEvents();
    draw();
  }
}

void VulkanEngine::init_vulkan() {
  vkb::InstanceBuilder builder;

  auto inst_ret = builder.set_app_name("Example Vulkan Application")
                      .request_validation_layers(true)
                      .require_api_version(1, 1, 0)
                      .use_default_debug_messenger()
                      .build();

  vkb::Instance vkb_inst = inst_ret.value();

  // store the instance
  _instance = vkb_inst.instance;
  // store the debug messenger
  _debug_messenger = vkb_inst.debug_messenger;

  glfwCreateWindowSurface(_instance, _window, nullptr, &_surface);

  vkb::PhysicalDeviceSelector selector{vkb_inst};
  vkb::PhysicalDevice physicalDevice = selector.set_minimum_version(1, 1).set_surface(_surface).select().value();

  vkb::DeviceBuilder deviceBuilder{physicalDevice};
  vkb::Device vkbDevice = deviceBuilder.build().value();

  _device = vkbDevice.device;
  _chosenGPU = physicalDevice.physical_device;

  _graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
  _graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
}

void VulkanEngine::init_swapchain() {
  vkb::SwapchainBuilder swapchainBuilder{_chosenGPU, _device, _surface};

  vkb::Swapchain vkbSwapchain = swapchainBuilder.use_default_format_selection()
                                    .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
                                    .set_desired_extent(_windowExtent.width, _windowExtent.height)
                                    .build()
                                    .value();

  _swapchain = vkbSwapchain.swapchain;
  _swapchainImages = vkbSwapchain.get_images().value();
  _swapchainImageViews = vkbSwapchain.get_image_views().value();
  _swapchainImageFormat = vkbSwapchain.image_format;
}

void VulkanEngine::init_commands() {
  VkCommandPoolCreateInfo commandPoolInfo =
      vkinit::command_pool_create_info(_graphicsQueueFamily, vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
  vkCreateCommandPool(_device, &commandPoolInfo, nullptr, &_commandPool);

  // allocate the default command buffer that we will use for rendering
  VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(_commandPool, 1);
  vkAllocateCommandBuffers(_device, &cmdAllocInfo, &_mainCommandBuffer);
}

void VulkanEngine::init_default_renderpass() {
  // the renderpass will use this color attachment.
  VkAttachmentDescription color_attachment = {};
  // the attachment will have the format needed by the swapchain
  color_attachment.format = _swapchainImageFormat;
  // 1 sample, we won't be doing MSAA
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  // we Clear when this attachment is loaded
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  // we keep the attachment stored when the renderpass ends
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  // we don't care about stencil
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

  // we don't know or care about the starting layout of the attachment
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

  // after the renderpass ends, the image has to be on a layout ready for display
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference color_attachment_ref = {};
  // attachment number will index into the pAttachments array in the parent renderpass itself
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  // we are going to create 1 subpass, which is the minimum you can do
  VkSubpassDescription subpass = {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &color_attachment_ref;

  VkRenderPassCreateInfo render_pass_info = {};
  render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

  // connect the color attachment to the info
  render_pass_info.attachmentCount = 1;
  render_pass_info.pAttachments = &color_attachment;
  // connect the subpass to the info
  render_pass_info.subpassCount = 1;
  render_pass_info.pSubpasses = &subpass;

  vkCreateRenderPass(_device, &render_pass_info, nullptr, &_renderPass);
}

void VulkanEngine::init_framebuffers() {
  // create the framebuffers for the swapchain images. This will connect the render-pass to the images for rendering
  VkFramebufferCreateInfo fb_info = {};
  fb_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  fb_info.pNext = nullptr;

  fb_info.renderPass = _renderPass;
  fb_info.attachmentCount = 1;
  fb_info.width = _windowExtent.width;
  fb_info.height = _windowExtent.height;
  fb_info.layers = 1;

  // grab how many images we have in the swapchain
  const uint32_t swapchain_imagecount = _swapchainImages.size();
  _framebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);

  // create framebuffers for each of the swapchain image views
  for (int i = 0; i < swapchain_imagecount; i++) {
    fb_info.pAttachments = &_swapchainImageViews[i];
    vkCreateFramebuffer(_device, &fb_info, nullptr, &_framebuffers[i]);
  }
}

void VulkanEngine::init_sync_structures() {
  // create synchronization structures

  VkFenceCreateInfo fenceCreateInfo = {};
  fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceCreateInfo.pNext = nullptr;

  // we want to create the fence with the Create Signaled flag, so we can wait on it before using it on a GPU command
  // (for the first frame)
  fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  vkCreateFence(_device, &fenceCreateInfo, nullptr, &_renderFence);

  // for the semaphores we don't need any flags
  VkSemaphoreCreateInfo semaphoreCreateInfo = {};
  semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  semaphoreCreateInfo.pNext = nullptr;
  semaphoreCreateInfo.flags = 0;

  vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_presentSemaphore);
  vkCreateSemaphore(_device, &semaphoreCreateInfo, nullptr, &_renderSemaphore);
}

bool VulkanEngine::load_shader_module(const char *filePath,
                                      VkShaderModule *outShaderModule) {  // open the file. With cursor at the end
  std::ifstream file(filePath, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    return false;
  }

  // find what the size of the file is by looking up the location of the cursor
  // because the cursor is at the end, it gives the size directly in bytes
  size_t fileSize = (size_t)file.tellg();

  // spirv expects the buffer to be on uint32, so make sure to reserve an int vector big enough for the entire file
  std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

  // put file cursor at beginning
  file.seekg(0);

  // load the entire file into the buffer
  file.read((char *)buffer.data(), fileSize);

  // now that the file is loaded into the buffer, we can close it
  file.close();

  // create a new shader module, using the buffer we loaded
  VkShaderModuleCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.pNext = nullptr;

  // codeSize has to be in bytes, so multiply the ints in the buffer by size of int to know the real size of the buffer
  createInfo.codeSize = buffer.size() * sizeof(uint32_t);
  createInfo.pCode = buffer.data();

  // check that the creation goes well.
  VkShaderModule shaderModule;
  if (vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
    return false;
  }
  *outShaderModule = shaderModule;
  return true;
}

void VulkanEngine::init_pipelines() {
  VkShaderModule triangleFragShader;
  if (!load_shader_module("./shaders/triangle.frag.spv", &triangleFragShader)) {
    std::cout << "Error when building the triangle fragment shader module" << std::endl;
  } else {
    std::cout << "Triangle fragment shader successfully loaded" << std::endl;
  }

  VkShaderModule triangleVertexShader;
  if (!load_shader_module("./shaders/triangle.vert.spv", &triangleVertexShader)) {
    std::cout << "Error when building the triangle vertex shader module" << std::endl;

  } else {
    std::cout << "Triangle vertex shader successfully loaded" << std::endl;
  }

  VkPipelineLayoutCreateInfo pipeline_layout_info = vkinit::pipeline_layout_create_info();

  vkCreatePipelineLayout(_device, &pipeline_layout_info, nullptr, &_trianglePipelineLayout);

  // build the stage-create-info for both vertex and fragment stages. This lets the pipeline know the shader modules per
  // stage
  PipelineBuilder pipelineBuilder;

  pipelineBuilder._shaderStages.push_back(
      vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, triangleVertexShader));

  pipelineBuilder._shaderStages.push_back(
      vkinit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader));

  // vertex input controls how to read vertices from vertex buffers. We aren't using it yet
  pipelineBuilder._vertexInputInfo = vkinit::vertex_input_state_create_info();

  // input assembly is the configuration for drawing triangle lists, strips, or individual points.
  // we are just going to draw triangle list
  pipelineBuilder._inputAssembly = vkinit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

  // build viewport and scissor from the swapchain extents
  pipelineBuilder._viewport.x = 0.0f;
  pipelineBuilder._viewport.y = 0.0f;
  pipelineBuilder._viewport.width = (float)_windowExtent.width;
  pipelineBuilder._viewport.height = (float)_windowExtent.height;
  pipelineBuilder._viewport.minDepth = 0.0f;
  pipelineBuilder._viewport.maxDepth = 1.0f;

  pipelineBuilder._scissor.offset = {0, 0};
  pipelineBuilder._scissor.extent = _windowExtent;

  // configure the rasterizer to draw filled triangles
  pipelineBuilder._rasterizer = vkinit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);

  // we don't use multisampling, so just run the default one
  pipelineBuilder._multisampling = vkinit::multisampling_state_create_info();

  // a single blend attachment with no blending and writing to RGBA
  pipelineBuilder._colorBlendAttachment = vkinit::color_blend_attachment_state();

  // use the triangle layout we created
  pipelineBuilder._pipelineLayout = _trianglePipelineLayout;

  // finally build the pipeline
  _trianglePipeline = pipelineBuilder.build_pipeline(_device, _renderPass);
}

VkPipeline PipelineBuilder::build_pipeline(
    VkDevice device, VkRenderPass pass) {  // make viewport state from our stored viewport and scissor.
  // at the moment we won't support multiple viewports or scissors
  VkPipelineViewportStateCreateInfo viewportState = {};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.pNext = nullptr;

  viewportState.viewportCount = 1;
  viewportState.pViewports = &_viewport;
  viewportState.scissorCount = 1;
  viewportState.pScissors = &_scissor;

  // setup dummy color blending. We aren't using transparent objects yet
  // the blending is just "no blend", but we do write to the color attachment
  VkPipelineColorBlendStateCreateInfo colorBlending = {};
  colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.pNext = nullptr;

  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &_colorBlendAttachment;

  // build the actual pipeline
  // we now use all of the info structs we have been writing into into this one to create the pipeline
  VkGraphicsPipelineCreateInfo pipelineInfo = {};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.pNext = nullptr;

  pipelineInfo.stageCount = _shaderStages.size();
  pipelineInfo.pStages = _shaderStages.data();
  pipelineInfo.pVertexInputState = &_vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &_inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &_rasterizer;
  pipelineInfo.pMultisampleState = &_multisampling;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.layout = _pipelineLayout;
  pipelineInfo.renderPass = pass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

  // it's easy to error out on create graphics pipeline, so we handle it a bit better than the common VK_CHECK case
  VkPipeline newPipeline;
  if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS) {
    std::cout << "failed to create pipeline\n";
    return VK_NULL_HANDLE;  // failed to create graphics pipeline
  } else {
    return newPipeline;
  }
}