#pragma once
#include "Common.h"
#include "vulkan/vulkan.hpp"

class VulkanUtils {
public:
	static std::vector<const char*> filterExtensionNames(const std::vector<VkExtensionProperties>& properties, const std::vector<const char*>& requiredNames);
	static std::vector<const char*> filterLayerNames(const std::vector<VkLayerProperties>& properties, const std::vector<const char*>& requiredNames);

	static int cmpDeviceType(VkPhysicalDeviceType left, VkPhysicalDeviceType right);

	static void allocCommandBuffer(const VkDevice* device, const VkCommandPool cmdPool, VkCommandBuffer* cmdBuf, const VkCommandBufferAllocateInfo* commandBufferInfo = NULL);
	static void beginCommandBuffer(VkCommandBuffer cmdBuf, VkCommandBufferBeginInfo* inCmdBufInfo = NULL);
	static void endCommandBuffer(VkCommandBuffer cmdBuf);
	static void submitCommandBuffer(const VkQueue& queue, const VkCommandBuffer* cmdBufList, const VkSubmitInfo* submitInfo = NULL, const VkFence& fence = VK_NULL_HANDLE);

};

