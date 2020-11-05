#include "Pipeline.h"
#include "VulkanFunctionPointers.h"

namespace VulTerGen
{
	Pipeline::Pipeline(Device* device, Swapchain* swapchain, RenderPass* renderPass)
	{
			this->device = device;
			this->swapchain = swapchain;
			this->renderPass = renderPass;

			Shader vertexShader(device->logicalDevice, "D:\\Projects\\VulTerGen\\VulkanVisual\\shaders\\vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
			Shader fragmentShader(device->logicalDevice, "D:\\Projects\\VulTerGen\\VulkanVisual\\shaders\\frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);

			pipelineShaderStageCreateInfos.push_back(vertexShader.shaderStageCreateInfo);
			pipelineShaderStageCreateInfos.push_back(fragmentShader.shaderStageCreateInfo);

			AddVertexInputBindingDescription();
			AddVertexInputAttributeDescription();

			SetVertexInputState();
			SetInputAssemblyState();
			SetViewportState();
			SetRasterizationState();
			CreatePipelineLayout();
			SetColorBlendState();
			SetMultisampleState();

			CreateGraphicPipeline();
	}

	Pipeline::~Pipeline()
	{
		DestoryGraphicPipeline();
		DestroyPipelineLayout();
	}

	void Pipeline::AddVertexInputBindingDescription()
	{
		//Defines the verts x, y ,z component4
		VkVertexInputBindingDescription vertexInputBindingDescription = {
			vertexInputBindingDescription.binding = 0, //where the data is taken from (from which binding)
			vertexInputBindingDescription.stride = 3 * sizeof(float), //what is the stride between consecutive elements in the buffer pos.x, pos.y, pos.z, //col.x, col.y, col.z
			vertexInputBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX //how this data is read
		};

		vertexInputBindingDescriptions.push_back(vertexInputBindingDescription);
	}

	void Pipeline::AddVertexInputAttributeDescription()
	{
		//Defines where these 
		VkVertexInputAttributeDescription vertexInputPosAttributeDescription = {
			vertexInputPosAttributeDescription.location = 0,
			vertexInputPosAttributeDescription.binding = 0,
			vertexInputPosAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT,
			vertexInputPosAttributeDescription.offset = 0
		};

		vertexInputAttributeDescriptions.push_back(vertexInputPosAttributeDescription);
	}

	void Pipeline::SetVertexInputState()
	{
		pipelineVertexInputStateCreateInfo =
		{
			pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			pipelineVertexInputStateCreateInfo.pNext = nullptr,
			pipelineVertexInputStateCreateInfo.flags = 0,
			pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexInputBindingDescriptions.size(),						//1,
			pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vertexInputBindingDescriptions.data(),							//&vertexInputBindingDescription,
			pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexInputAttributeDescriptions.size(),					//vertexInputAttribDescriptions.size(),
			pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexInputAttributeDescriptions.data()						//vertexInputAttribDescriptions.data()
		};
	}

	void Pipeline::SetInputAssemblyState()
	{
		pipelineInputAssemblyStateCreateInfo = {
			pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			pipelineInputAssemblyStateCreateInfo.pNext = nullptr,
			pipelineInputAssemblyStateCreateInfo.flags = 0,
			pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
			pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE
		};
	}

	void Pipeline::SetViewportState()
	{
		viewport = {
			viewport.x = 0.0f,
			viewport.y = 0.0f,
			viewport.width = swapchain->surfaceCapabilities.currentExtent.width,
			viewport.height = swapchain->surfaceCapabilities.currentExtent.height,
			viewport.minDepth = 0.0f,
			viewport.maxDepth = 1.0f
		};

		scissor = {
			scissor.offset = {0, 0},
			scissor.extent = swapchain->surfaceCapabilities.currentExtent
		};

		pipelineViewportStateCreateInfo = {
			pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			pipelineViewportStateCreateInfo.pNext = nullptr,
			pipelineViewportStateCreateInfo.flags = 0,
			pipelineViewportStateCreateInfo.viewportCount = 1,
			pipelineViewportStateCreateInfo.pViewports = &viewport,
			pipelineViewportStateCreateInfo.scissorCount = 1,
			pipelineViewportStateCreateInfo.pScissors = &scissor
		};
	}

	void Pipeline::SetRasterizationState()
	{
		pipelineRasterizerStateCreateInfo = {
			pipelineRasterizerStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
			pipelineRasterizerStateCreateInfo.pNext = nullptr,
			pipelineRasterizerStateCreateInfo.flags = 0,
			pipelineRasterizerStateCreateInfo.depthClampEnable = VK_FALSE, //REQUIRES FEATURE If True Fragments that are beyond the near and far planes are clamped to them as opposed to discarding them
			pipelineRasterizerStateCreateInfo.rasterizerDiscardEnable = VK_FALSE, //If True it won't allow the geometry to further pass through to later stages
			pipelineRasterizerStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL,
			pipelineRasterizerStateCreateInfo.cullMode = VK_CULL_MODE_NONE,
			pipelineRasterizerStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE, //clockwise or counterclockwise
			pipelineRasterizerStateCreateInfo.depthBiasEnable = VK_FALSE, //used for shadow mapping
			pipelineRasterizerStateCreateInfo.depthBiasConstantFactor = 0.0f, //used for shadow mapping
			pipelineRasterizerStateCreateInfo.depthBiasClamp = 0.0f, //used for shadow mapping
			pipelineRasterizerStateCreateInfo.depthBiasSlopeFactor = 0.0f, //used for shadow mapping
			pipelineRasterizerStateCreateInfo.lineWidth = 1.0f //Not sure
		};
	}

	void Pipeline::SetColorBlendState()
	{
		blendAttachmentState[0].colorWriteMask = 0xf;
		blendAttachmentState[0].blendEnable = VK_FALSE;

		colorBlendStateCreateInfo = {
			colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			colorBlendStateCreateInfo.pNext = nullptr,
			colorBlendStateCreateInfo.flags = 0,
			colorBlendStateCreateInfo.logicOpEnable = VK_FALSE,
			colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_CLEAR,
			colorBlendStateCreateInfo.attachmentCount = 1,					//1
			colorBlendStateCreateInfo.pAttachments = blendAttachmentState,	//blendAttachmentState
			colorBlendStateCreateInfo.blendConstants[0] = 0.0f,
			colorBlendStateCreateInfo.blendConstants[1] = 0.0f,
			colorBlendStateCreateInfo.blendConstants[2] = 0.0f,
			colorBlendStateCreateInfo.blendConstants[3] = 0.0f
		};
	}

	void Pipeline::SetMultisampleState()
	{
		multisampleStateCreateInfo = {
			multisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			multisampleStateCreateInfo.pNext = nullptr,
			multisampleStateCreateInfo.flags = 0,
			multisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			multisampleStateCreateInfo.sampleShadingEnable = VK_FALSE,
			multisampleStateCreateInfo.minSampleShading = 0.0f,
			multisampleStateCreateInfo.pSampleMask = nullptr,
			multisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE,
			multisampleStateCreateInfo.alphaToOneEnable = VK_FALSE
		};
	}

	void Pipeline::CreatePipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(float) * 4;

		//VkDescriptorSetLayoutBinding uboTransformLayout =
		//{
		//	uboTransformLayout.binding = 0,
		//	uboTransformLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		//	uboTransformLayout.descriptorCount = 1,
		//	uboTransformLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		//	uboTransformLayout.pImmutableSamplers = nullptr
		//};

		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo =
		{
			descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			descriptorSetLayoutInfo.pNext = nullptr,
			descriptorSetLayoutInfo.flags = 0,
			descriptorSetLayoutInfo.bindingCount = 0,//1,
			descriptorSetLayoutInfo.pBindings = nullptr//&uboTransformLayout
		};

		vkCreateDescriptorSetLayout(device->logicalDevice, &descriptorSetLayoutInfo, nullptr, &descriptorSetLayout);

		pipelineLayoutCreateInfo = {
			pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			pipelineLayoutCreateInfo.pNext = nullptr,
			pipelineLayoutCreateInfo.flags = 0,
			pipelineLayoutCreateInfo.setLayoutCount = 1,
			pipelineLayoutCreateInfo.pSetLayouts = &descriptorSetLayout,
			pipelineLayoutCreateInfo.pushConstantRangeCount = 1,
			pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange
		};

		vkCreatePipelineLayout(device->logicalDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
	}

	void Pipeline::DestroyPipelineLayout()
	{
		if (pipelineLayout != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorSetLayout(device->logicalDevice, descriptorSetLayout, nullptr);
			vkDestroyPipelineLayout(device->logicalDevice, pipelineLayout, nullptr);
			pipelineLayout = VK_NULL_HANDLE;
		}
	}

	void Pipeline::CreateGraphicPipeline()
	{
		graphicPipelineCreateInfo = {
			graphicPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			graphicPipelineCreateInfo.pNext = nullptr,
			graphicPipelineCreateInfo.flags = 0,
			graphicPipelineCreateInfo.stageCount = pipelineShaderStageCreateInfos.size(), //The number of shader modules (Vertex stage, Fragment Stage)
			graphicPipelineCreateInfo.pStages = pipelineShaderStageCreateInfos.data(),
			graphicPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo,
			graphicPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo,
			graphicPipelineCreateInfo.pTessellationState = nullptr,
			graphicPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo,
			graphicPipelineCreateInfo.pRasterizationState = &pipelineRasterizerStateCreateInfo,
			graphicPipelineCreateInfo.pMultisampleState = &multisampleStateCreateInfo,
			graphicPipelineCreateInfo.pDepthStencilState = nullptr,
			graphicPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo,
			graphicPipelineCreateInfo.pDynamicState = nullptr,
			graphicPipelineCreateInfo.layout = pipelineLayout,
			graphicPipelineCreateInfo.renderPass = renderPass->renderPass,
			graphicPipelineCreateInfo.subpass = 0,
			graphicPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE,
			graphicPipelineCreateInfo.basePipelineIndex = -1
		};

		vkCreateGraphicsPipelines(device->logicalDevice, nullptr, 1, &graphicPipelineCreateInfo, nullptr, &graphicPipeline);
	}

	void Pipeline::DestoryGraphicPipeline()
	{
		if (graphicPipeline != VK_NULL_HANDLE)
		{
			vkDestroyPipeline(device->logicalDevice, graphicPipeline, nullptr);
			graphicPipeline = VK_NULL_HANDLE;
		}
	}
}