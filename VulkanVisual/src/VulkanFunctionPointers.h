#pragma once
#define VK_NO_PROTOTYPE
#include "vulkan/vulkan.hpp"

#ifndef INSIDE_TRANSLATION_UNIT
	#define EXPORTED_VULKAN_FUNCTION( name ) extern PFN_##name name; 
	#define GLOBAL_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) extern PFN_##name name;
#else
	#define EXPORTED_VULKAN_FUNCTION( name ) PFN_##name name; 
	#define GLOBAL_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name ) PFN_##name name;
#endif


namespace VulTerGen
{
	bool LoadVulkanLibrary();
	void LoadInstanceLevelFunctions();
	void LoadDeviceLevelFunctions();
	void FreeVulkanLibrary();

	//PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
	EXPORTED_VULKAN_FUNCTION(vkGetInstanceProcAddr)

	GLOBAL_LEVEL_VULKAN_FUNCTION(vkEnumerateInstanceExtensionProperties)
	GLOBAL_LEVEL_VULKAN_FUNCTION(vkEnumerateInstanceLayerProperties)
	GLOBAL_LEVEL_VULKAN_FUNCTION(vkCreateInstance)

	INSTANCE_LEVEL_VULKAN_FUNCTION(vkDestroyInstance)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkCreateDevice)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkDestroyDevice)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkEnumeratePhysicalDevices)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetPhysicalDeviceProperties)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetPhysicalDeviceMemoryProperties)

	INSTANCE_LEVEL_VULKAN_FUNCTION(vkEnumerateDeviceExtensionProperties)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetPhysicalDeviceFeatures)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)

	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetDeviceQueue)

	INSTANCE_LEVEL_VULKAN_FUNCTION(vkCreateWin32SurfaceKHR)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkCreateSwapchainKHR)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkDestroySwapchainKHR)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetSwapchainImagesKHR)
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkAcquireNextImageKHR)
		
	INSTANCE_LEVEL_VULKAN_FUNCTION(vkGetDeviceProcAddr)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkCreateSemaphore)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkDestroySemaphore)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkCreateFence)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkWaitForFences)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkResetFences)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkQueuePresentKHR)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkQueueSubmit)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkDestroyFence)	
	DEVICE_LEVEL_VULKAN_FUNCTION(vkDeviceWaitIdle)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkQueueWaitIdle)
		
	DEVICE_LEVEL_VULKAN_FUNCTION(vkGetBufferMemoryRequirements)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkAllocateMemory)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkCreateBuffer)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkBindBufferMemory)

	DEVICE_LEVEL_VULKAN_FUNCTION(vkCreateCommandPool)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkAllocateCommandBuffers)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkFreeCommandBuffers)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkBeginCommandBuffer)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkEndCommandBuffer)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkResetCommandBuffer)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkResetCommandPool)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkDestroyCommandPool)

	DEVICE_LEVEL_VULKAN_FUNCTION(vkCreateShaderModule)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkDestroyShaderModule)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkCreatePipelineLayout)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkDestroyPipelineLayout)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkCreateRenderPass)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkDestroyRenderPass)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkCreateGraphicsPipelines)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkDestroyPipeline)

	DEVICE_LEVEL_VULKAN_FUNCTION(vkCmdPipelineBarrier)
	DEVICE_LEVEL_VULKAN_FUNCTION(vkCmdClearColorImage)
};

#undef EXPORTED_VULKAN_FUNCTION
#undef GLOBAL_LEVEL_VULKAN_FUNCTION
#undef INSTANCE_LEVEL_VULKAN_FUNCTION
#undef DEVICE_LEVEL_VULKAN_FUNCTION