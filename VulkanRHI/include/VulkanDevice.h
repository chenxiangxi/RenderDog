#pragma once
#include "Common.h"
#include "vulkan/vulkan.hpp"

class VulkanDevice {
public:
	VulkanDevice(){}
	VulkanDevice(const vk::PhysicalDevice& physicalDevice);
	void init(const std::vector<const char*>& requiredDeviceExtensionNames);

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
	void setQueueFamilyIndex();

	bool m_inited;
	vk::PhysicalDevice m_physicalDevice;
	vk::Device m_device;
	std::vector<const char*> m_deviceExtensionNames;
	uint32_t m_queueFamilyIndex;
};