#pragma once
#include "Common.h"
#include "vulkan/vulkan.hpp"

class VulkanApplication {
public:
	void init();
	void destroy();
private:
	void createInstance();
	std::vector<const char*> getInstanceExtensionProperties(const std::vector<const char*> requiredExtensionProperties);
	std::vector<const char*> updateInstanceExtensionProperties();
	std::vector<const char*> getInstanceLayerProperties(const std::vector<const char*> requiredLayerProperties);
	std::vector<const char*> updateInstanceLayerProperties();

	vk::Instance m_instance;
};