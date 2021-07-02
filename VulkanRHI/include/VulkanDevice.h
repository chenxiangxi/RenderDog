#pragma once
#include "Common.h"
#include "vulkan/vulkan.hpp"

class VulkanDevice {
public:
	VulkanDevice(){}
	VulkanDevice(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice);
	void init(const std::vector<const char*>& requiredDeviceExtensionNames);
	void createSurface(HWND hwnd);

	vk::PhysicalDevice getPhysicalDevice() {
		return m_physicalDevice;
	}

	const std::vector<const char*>& getDeviceExtensionNames() {
		return m_deviceExtensionNames;
	}

	void setDeviceExtensionNames(const std::vector<const char*>& exNames) {
		m_deviceExtensionNames = exNames;
	}

private:
	std::vector<const char*> getDeviceExtensionNames(const std::vector<const char*>& requiredNames);
	void initQueueFamily();
	void createCommandBuffer();
	

	bool m_inited = false;

	vk::Instance m_instance;
	vk::PhysicalDevice m_physicalDevice;
	vk::Device m_device;
	vk::SurfaceKHR m_surface;
	std::vector<const char*> m_deviceExtensionNames;

	vk::CommandPool m_commandPool;

	uint32_t m_queueFamilyIndex;
	vk::Queue m_queue;
};