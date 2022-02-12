#pragma once
#include "Common.h"
#include <map>
#include "vulkan/vulkan.hpp"

class VulkanDevice {
public:
	VulkanDevice(const VkInstance& instance, const VkPhysicalDevice& physicalDevice);
	void init(const std::vector<const char*>& requiredDeviceExtensionNames, void* hwnd = nullptr);
	
	VkPhysicalDevice getPhysicalDevice() const{
		return m_physicalDevice;
	}

	const std::vector<const char*>& getDeviceExtensionNames() {
		return m_deviceExtensionNames;
	}

	void setDeviceExtensionNames(const std::vector<const char*>& exNames) {
		m_deviceExtensionNames = exNames;
	}

	static std::shared_ptr<VulkanDevice> findOpticalDevice(const std::vector<std::shared_ptr<VulkanDevice>>& devices);

private:
	std::vector<const char*> getDeviceExtensionNames(const std::vector<const char*>& requiredNames);
	void findQueueFamily();
	VkResult createCommandBuffer();
	void createSurface(void* hwnd);
	void createSwapchain();
	void createDepthImage();
	void createDevice(const std::vector<const char*>& requiredDeviceExtensionNames);

	bool m_inited = false;

	VkInstance m_instance;
	VkPhysicalDevice m_physicalDevice;
	VkDevice m_device;

	VkSurfaceKHR m_surface;
	VkFormat m_surfaceFormat;
	std::vector<VkSurfaceFormatKHR> m_surfaceSupportedFormats;
	uint32_t m_surfaceWidth = 1024;
	uint32_t m_surfaceHeight = 756;

	VkSwapchainKHR m_swapchain;
	std::vector<VkImage> m_swapchainImages;
	std::vector<VkImageView> m_swapchainImageViews;
	struct {
		VkFormat format = VK_FORMAT_D16_UNORM;
		VkImage image;
		VkDeviceMemory memory;
		VkImageView view;
	} m_depthImage;
	VkCommandPool m_commandPool;
	VkCommandBuffer m_depthImageCmdBuffer;
	
	std::vector<const char*> m_deviceExtensionNames;
	std::optional<uint32_t> m_graphicQueueFamilyIndex;
	std::optional<uint32_t> m_presentQueueFamilyIndex;
	VkQueue m_queue;
};