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
		CreateShader(filename, type);
	}

	Shader::~Shader()
	{
		DestroyShader();
	}

	void Shader::CreateShader(const std::string& filename, VkShaderStageFlagBits type)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!");
		}
		size_t filesize = (size_t)file.tellg();
		shaderBinary.resize(filesize);
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

		vkCreateShaderModule(logicalDevice, &shaderModuleCreateInfo, nullptr, &module);

		shaderStageCreateInfo = {
			shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			shaderStageCreateInfo.pNext = nullptr,
			shaderStageCreateInfo.flags = 0,
			shaderStageCreateInfo.stage = type,
			shaderStageCreateInfo.module = module,
			shaderStageCreateInfo.pName = "main",
			shaderStageCreateInfo.pSpecializationInfo = nullptr
		};
	}

	void Shader::DestroyShader()
	{
		if (module != nullptr)
			vkDestroyShaderModule(logicalDevice, module, nullptr);
	}
}
