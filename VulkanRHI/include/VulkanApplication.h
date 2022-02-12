#pragma once
#include "Common.h"
#include "vulkan/vulkan.hpp"
#include "VulkanDevice.h"

class VulkanApplication {
public:
	void init(const std::vector<const char*>& requiredInstanceExtensionNames = { VK_KHR_SURFACE_EXTENSION_NAME },
		const std::vector<const char*>& requiredInstanceLayerNames = { "VK_LAYER_LUNARG_api_dump" },
		const std::vector<const char*>& requiredDeviceExtensionNames = {}, void* hwnd = nullptr);

	void render();
	void destroy();

	const std::vector<const char*>& getInstanceExtensionNames() {
		return m_instanceExtensionNames;
	}

	void setInstanceExtensionNames(const std::vector<const char*>& exNames) {
		m_instanceExtensionNames = exNames;
	}

	const std::vector<const char*>& getInstanceLayerNames() {
		return m_instanceLayerNames;
	}

	void setInstanceLayerNames(const std::vector<const char*>& layerNames) {
		m_instanceLayerNames = layerNames;
	}

private:
	void createInstance(void* hwnd);
	void createDeviceDefault(void* hwnd);
	void enumerateSuitablePhysicalDevices(void* hwnd);

	std::vector<const char*> getInstanceExtensionNames(const std::vector<const char*>& requiredNames);
	std::vector<const char*> getInstanceLayerNames(const std::vector<const char*>& requiredNames);
	std::vector<const char*> filterExtensionNames(const std::vector<VkExtensionProperties>& properties, const std::vector<const char*>& requiredNames);
	std::vector<const char*> filterLayerNames(const std::vector<VkLayerProperties>& properties, const std::vector<const char*>& requiredNames);

	VkInstance m_instance;
	std::vector<std::shared_ptr<VulkanDevice>> m_devices;
	std::shared_ptr<VulkanDevice> m_currentDevice;

	std::vector<const char*> m_instanceExtensionNames;
	std::vector<const char*> m_instanceLayerNames;
	std::vector<const char*> m_requiredInstanceExtensionNames;
	std::vector<const char*> m_requiredInstanceLayerNames;
	std::vector<const char*> m_requiredDeviceExtensionNames;
};