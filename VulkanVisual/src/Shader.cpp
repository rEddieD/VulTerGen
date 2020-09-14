#include <fstream>
#include <vulkan/vulkan.hpp>
#include "Shader.h"

namespace VulTerGen
{
	Shader::Shader(VkDevice logicalDevice, const std::string& filename, VkShaderStageFlagBits type)
	{
		this->logicalDevice = logicalDevice;
		this->shader = CreateShader(filename, type);
	}

	Shader::~Shader()
	{
		DestroyShader(shader);
	}

	VkShaderModule Shader::CreateShader(const std::string& filename, VkShaderStageFlagBits type)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		size_t filesize = (size_t)file.tellg();
		std::vector<char> shaderBinary(filesize);
		file.seekg(0);
		file.read(shaderBinary.data(), filesize);
		file.close();

		VkShaderModuleCreateInfo shaderModuleCreateInfo = {
				shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
				shaderModuleCreateInfo.pNext = nullptr,
				shaderModuleCreateInfo.flags = 0,
				shaderModuleCreateInfo.codeSize = shaderBinary.size(),
				shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(shaderBinary.data())
		};

		VkShaderModule shaderModule;

		vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, nullptr, &shaderModule);

		return shaderModule;
	}

	void Shader::DestroyShader(VkShaderModule shaderModule)
	{
		vkDestroyShaderModule(logicalDevice, shaderModule, nullptr);
	}
}
