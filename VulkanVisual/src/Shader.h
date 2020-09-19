#pragma once

namespace VulTerGen 
{
	class Shader
	{
	private:
		VkDevice logicalDevice;
	public:
		VkShaderModule module;
		VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
		Shader(VkDevice logicalDevice, const std::string& filename, VkShaderStageFlagBits type);
		virtual ~Shader();
		void CreateShader(const std::string& filename, VkShaderStageFlagBits type);
		void DestroyShader();
	};
}