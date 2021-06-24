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
	vk::Instance m_instance;
};