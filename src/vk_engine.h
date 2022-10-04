//
// Created by Michael Stengel on 10/3/2022.
//

#pragma once

#include <vk_types.h>

struct GLFWwindow;

class VulkanEngine {
 public:
  int _frameNumber{0};

  VkInstance _instance;
  VkDebugUtilsMessengerEXT _debug_messenger;
  VkPhysicalDevice _chosenGPU;
  VkDevice _device;
  VkSurfaceKHR _surface;

  VkSwapchainKHR _swapchain;
  VkFormat _swapchainImageFormat;
  std::vector<VkImage> _swapchainImages;
  std::vector<VkImageView> _swapchainImageViews;

  VkQueue _graphicsQueue;
  uint32_t _graphicsQueueFamily;

  VkCommandPool _commandPool;
  VkCommandBuffer _mainCommandBuffer;

  VkRenderPass _renderPass;
  std::vector<VkFramebuffer> _framebuffers;

  VkSemaphore _presentSemaphore, _renderSemaphore;
  VkFence _renderFence;

  VkPipelineLayout _trianglePipelineLayout;
  VkPipeline _trianglePipeline;

  VkExtent2D _windowExtent{1700, 900};

  GLFWwindow* _window;

  // initializes everything in the engine
  void init();

  // shuts down the engine
  void cleanup();

  // draw loop
  void draw();

  // run main loop
  void run();

 private:
  void init_vulkan();

  void init_swapchain();

  void init_commands();

  void init_default_renderpass();

  void init_framebuffers();

  void init_sync_structures();

  void init_pipelines();

  bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);
};

class PipelineBuilder {
 public:
  std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
  VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
  VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
  VkViewport _viewport;
  VkRect2D _scissor;
  VkPipelineRasterizationStateCreateInfo _rasterizer;
  VkPipelineColorBlendAttachmentState _colorBlendAttachment;
  VkPipelineMultisampleStateCreateInfo _multisampling;
  VkPipelineLayout _pipelineLayout;

  VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
};