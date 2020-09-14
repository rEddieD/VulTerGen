#pragma once

namespace VulTerGen 
{
	class Shader
	{
	public:
		Shader() = default;
		~Shader() = default;
		VkShaderModule CreateShader(const std::string& filename, VkShaderStageFlagBits type);
		void DestroyShader(VkShaderModule shaderModule);
	};
}