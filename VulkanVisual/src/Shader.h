#pragma once

namespace VulTerGen 
{
	class Shader
	{
	private:
		VkDevice logicalDevice;
		VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
	public:
		VkShaderModule module;
		Shader(VkDevice logicalDevice, const std::string& filename, VkShaderStageFlagBits type);
		virtual ~Shader();
		VkShaderModule CreateShader(const std::string& filename, VkShaderStageFlagBits type);
		void DestroyShader();
	};
}