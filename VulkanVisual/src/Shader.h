#pragma once
#include <vulkan/vulkan.hpp>
#include "VulkanFunctionPointers.h"

namespace VulTerGen 
{
	class Shader
	{
	private:
		VkDevice logicalDevice;
		std::vector<char> shaderBinary;

	public:
		VkShaderModule module;
		VkPipelineShaderStageCreateInfo shaderStageCreateInfo;

		Shader(VkDevice logicalDevice, const std::string& filename, VkShaderStageFlagBits type);
		virtual ~Shader();

		void CreateShader(const std::string& filename, VkShaderStageFlagBits type);
		void DestroyShader();

	};
}