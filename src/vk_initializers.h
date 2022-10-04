//
// Created by Michael Stengel on 10/3/2022.
//

#pragma once

#include <vk_types.h>

namespace vkinit {
vk::CommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, vk::CommandPoolCreateFlagBits flags);
vk::CommandBufferAllocateInfo command_buffer_allocate_info(
    VkCommandPool pool, uint32_t count = 1, vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);

VkPipelineShaderStageCreateInfo pipeline_shader_stage_create_info(VkShaderStageFlagBits stage,
                                                                  VkShaderModule shaderModule);
VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info();

VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info(VkPrimitiveTopology topology);

VkPipelineRasterizationStateCreateInfo rasterization_state_create_info(VkPolygonMode polygonMode);

VkPipelineMultisampleStateCreateInfo multisampling_state_create_info();

VkPipelineColorBlendAttachmentState color_blend_attachment_state();

VkPipelineLayoutCreateInfo pipeline_layout_create_info() ;
}  // namespace vkinit