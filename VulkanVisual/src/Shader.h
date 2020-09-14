#pragma once

namespace VulTerGen 
{
	class Shader
	{
	private:
		VkDevice logicalDevice;
		VkShaderModule shader;
	public:
		Shader(VkDevice logicalDevice, const std::string& filename, VkShaderStageFlagBits type);
		virtual ~Shader();
		VkShaderModule CreateShader(const std::string& filename, VkShaderStageFlagBits type);
		void DestroyShader(VkShaderModule shaderModule);
	};
}