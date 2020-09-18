#include <fstream>
#include <vulkan/vulkan.hpp>
#include "Shader.h"


namespace VulTerGen
{
	extern PFN_vkCreateShaderModule vkCreateShaderModule;
	extern PFN_vkDestroyShaderModule vkDestroyShaderModule;

	Shader::Shader(VkDevice logicalDevice, const std::string& filename, VkShaderStageFlagBits type)
	{
		this->logicalDevice = logicalDevice;
		this->module = CreateShader(filename, type);
	}

	Shader::~Shader()
	{
		DestroyShader();
	}

	VkShaderModule Shader::CreateShader(const std::string& filename, VkShaderStageFlagBits type)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
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


		shaderStageCreateInfo = {
			shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			shaderStageCreateInfo.pNext = nullptr,
			shaderStageCreateInfo.flags = 0,
			shaderStageCreateInfo.stage = type,
			shaderStageCreateInfo.module = module,
			shaderStageCreateInfo.pName = "main",
			shaderStageCreateInfo.pSpecializationInfo = nullptr
		};

		return shaderModule;
	}

	void Shader::DestroyShader()
	{
		vkDestroyShaderModule(logicalDevice, module, nullptr);
	}
}
