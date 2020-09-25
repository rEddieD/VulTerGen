#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include <Windows.h>
#include <vulkan/vulkan.hpp>
#include "VulkanFunctionPointers.h"
#include "Device.h"

namespace VulTerGen
{
	class Swapchain
	{
		VkInstance instance;

		Device* device;
		VkSurfaceKHR presentationSurface;
		VkSwapchainKHR swapchain;

		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainImageViews;

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		void CreatePresentationSurface(HWND hWnd);
		bool IsMailboxPresentationSupported();
		bool IsImagePresentationSupported();
		void GetSwapchainImage();
		void CreateImageViews();
		void DestroyImageViews();

		void CreateSwapchain();
		void DestroySwapchain();

		//Rendering--------------------------------------
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore commandBufferFinishedSemaphore;

		VkSubmitInfo submitInfo;
		VkPresentInfoKHR presentInfo;

		uint32_t swapchainImageIndex;
		VkPipelineStageFlags flags;
		//-----------------------------------------------
	public:
		Swapchain(VkInstance instance, Device* device, HWND hWnd);
		~Swapchain();

		void CreateFramebuffer(VkRenderPass renderPass);
		void DestroyFramebuffer();

		VkSemaphore CreateSemaphores();
		void DestroySemaphores(VkSemaphore semaphore);
		void SetupDraw(std::vector<VkCommandBuffer> commandBuffers);
		void Draw();
		void EndDraw();

		VkSurfaceFormatKHR surfaceFormat;
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		uint32_t graphicQueueFamilyIndex;
		std::vector<VkFramebuffer> swapchainFramebuffers;
	};
}
