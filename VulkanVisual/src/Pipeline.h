#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.hpp>
#include <vector>
#include "Shader.h"
#include "Device.h"
#include "Swapchain.h"
#include "RenderPass.h"
#include <glm/glm.hpp>

namespace VulTerGen
{
	class Pipeline
	{
	public:
		VkPipeline graphicPipeline;
		VkPipelineLayout pipelineLayout; //Pipeline layouts are used only in three situations--creating pipelines, binding descriptor sets, and updating push constants
		VkDescriptorSetLayout descriptorSetLayout;

		struct UniformBufferObject
		{
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};

		UniformBufferObject ubo;

	private:
		Device* device;
		Swapchain* swapchain;
		RenderPass* renderPass;
		
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo;

		std::vector<VkPipelineShaderStageCreateInfo> pipelineShaderStageCreateInfos;

		std::vector<VkVertexInputBindingDescription> vertexInputBindingDescriptions;
		std::vector<VkVertexInputAttributeDescription> vertexInputAttributeDescriptions;

		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo;
		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo;

		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo;
		
		VkPipelineRasterizationStateCreateInfo pipelineRasterizerStateCreateInfo;
		VkPipelineColorBlendAttachmentState blendAttachmentState[1];
		VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
		VkPipelineMultisampleStateCreateInfo multisampleStateCreateInfo;
		
		VkGraphicsPipelineCreateInfo graphicPipelineCreateInfo;

	public:
		Pipeline(Device* device, Swapchain* swapchain, RenderPass* renderpass);
		~Pipeline();

		void AddVertexInputBindingDescription();
		void AddVertexInputAttributeDescription();

		void SetVertexInputState();
		void SetInputAssemblyState();
		void SetViewportState();
		void SetRasterizationState();
		void SetColorBlendState();
		void SetMultisampleState();

		void CreatePipelineLayout();
		void DestroyPipelineLayout();
		void CreateGraphicPipeline();
		void DestoryGraphicPipeline();


		std::vector<VkBuffer> uniformBuffers;
		std::vector<VkDeviceMemory> uniformBuffersMemory;

		void AllocateUbo();

		VkDescriptorPool descriptorPool;
		void CreateDescriptorPool();
		void DestroyDescriptorPool();

		std::vector<VkDescriptorSet> descriptorSets;
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
	};
}