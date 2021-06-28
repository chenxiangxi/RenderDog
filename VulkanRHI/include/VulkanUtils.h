#pragma once
#include "Common.h"
#include "vulkan/vulkan.hpp"

class VulkanUtils {
public:
	static std::vector<const char*> filterExtensionNames(const std::vector<vk::ExtensionProperties>& properties, const std::vector<const char*>& requiredNames);
	static std::vector<const char*> filterLayerNames(const std::vector<vk::LayerProperties>& properties, const std::vector<const char*>& requiredNames);

	static int cmpDeviceType(vk::PhysicalDeviceType left, vk::PhysicalDeviceType right);
};