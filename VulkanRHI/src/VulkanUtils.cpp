#include "VulkanUtils.h"
#include <assert.h>


std::vector<const char*> VulkanUtils::filterExtensionNames(const std::vector<VkExtensionProperties>& properties, const std::vector<const char*>& requiredNames) {
    std::vector<const char*> propertyNames;

    std::for_each(requiredNames.begin(), requiredNames.end(), [&](const char* extName) {
        for (int i = 0; i < properties.size(); ++i) {
            if (std::strcmp(extName, properties[i].extensionName) == 0) {
                propertyNames.push_back(extName);
                break;
            }
        }
        });

    return propertyNames;
}

std::vector<const char*> VulkanUtils::filterLayerNames(const std::vector<VkLayerProperties>& properties, const std::vector<const char*>& requiredNames) {
    std::vector<const char*> propertyNames;

    std::for_each(requiredNames.begin(), requiredNames.end(), [&](const char* layerName) {
        for (int i = 0; i < properties.size(); ++i) {
            if (std::strcmp(layerName, properties[i].layerName) == 0) {
                propertyNames.push_back(layerName);
                break;
            }
        }
        });

    return propertyNames;
}

int VulkanUtils::cmpDeviceType(VkPhysicalDeviceType left, VkPhysicalDeviceType right) {
    auto getPriority = [](VkPhysicalDeviceType deviceType) {
        switch (deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return 0;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return 1;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return 2;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return 3;
        default:
            return 4;
        }
    };

    int leftPriority = getPriority(left);
    int rightPriority = getPriority(right);
    return leftPriority > rightPriority ? 1 : (leftPriority < rightPriority ? -1 : 0);
}

void VulkanUtils::allocCommandBuffer(const VkDevice* device, const VkCommandPool cmdPool, VkCommandBuffer* cmdBuf, const VkCommandBufferAllocateInfo* commandBufferInfo) {
    VkResult result;

    if (commandBufferInfo) {
        result = vkAllocateCommandBuffers(*device, commandBufferInfo, cmdBuf);
        assert(result == VK_SUCCESS);
        return;
    }

    VkCommandBufferAllocateInfo cmdInfo = {};
    cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdInfo.pNext = NULL;
    cmdInfo.commandPool = cmdPool;
    cmdInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdInfo.commandBufferCount = (uint32_t)sizeof(cmdBuf) / sizeof(VkCommandBuffer);;

    result = vkAllocateCommandBuffers(*device, &cmdInfo, cmdBuf);
    assert(result == VK_SUCCESS);
}

void VulkanUtils::beginCommandBuffer(VkCommandBuffer cmdBuf, VkCommandBufferBeginInfo* inCmdBufInfo) {
    // Dependency on  the initialieCommandBuffer()
    VkResult  result;
    // If the user has specified the custom command buffer use it
    if (inCmdBufInfo) {
        result = vkBeginCommandBuffer(cmdBuf, inCmdBufInfo);
        assert(result == VK_SUCCESS);
        return;
    }

    // Otherwise, use the default implementation.
    VkCommandBufferInheritanceInfo cmdBufInheritInfo = {};
    cmdBufInheritInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    cmdBufInheritInfo.pNext = NULL;
    cmdBufInheritInfo.renderPass = VK_NULL_HANDLE;
    cmdBufInheritInfo.subpass = 0;
    cmdBufInheritInfo.framebuffer = VK_NULL_HANDLE;
    cmdBufInheritInfo.occlusionQueryEnable = VK_FALSE;
    //cmdBufInheritInfo.queryFlags = nullptr;// VkQueryControlFlagBits::ePrecise;
    //cmdBufInheritInfo.pipelineStatistics = 0;

    VkCommandBufferBeginInfo cmdBufInfo = {};
    cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmdBufInfo.pNext = NULL;
    cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    cmdBufInfo.pInheritanceInfo = &cmdBufInheritInfo;

    result = vkBeginCommandBuffer(cmdBuf, &cmdBufInfo);
    assert(result == VK_SUCCESS);
}

void VulkanUtils::endCommandBuffer(VkCommandBuffer cmdBuf) {
    VkResult  result;
    result = vkEndCommandBuffer(cmdBuf);
    assert(result == VK_SUCCESS);
}

void VulkanUtils::submitCommandBuffer(const VkQueue& queue, const VkCommandBuffer* commandBuffer, const VkSubmitInfo* inSubmitInfo, const VkFence& fence) {
    VkResult result;

    // If Subimt information is avialable use it as it is, this assumes that 
    // the commands are already specified in the structure, hence ignore command buffer 
    if (inSubmitInfo) {
        result = vkQueueSubmit(queue, 1, inSubmitInfo, fence);
        assert(result == VK_SUCCESS);

        result = vkQueueWaitIdle(queue);
        assert(result == VK_SUCCESS);
        return;
    }

    // Otherwise, create the submit information with specified buffer commands
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO_KHR;
    submitInfo.pNext = NULL;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = NULL;
    submitInfo.pWaitDstStageMask = NULL;
    submitInfo.commandBufferCount = (uint32_t)sizeof(commandBuffer) / sizeof(VkCommandBuffer);
    submitInfo.pCommandBuffers = commandBuffer;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = NULL;

    result = vkQueueSubmit(queue, 1, &submitInfo, fence);
    assert(result == VkResult::VK_SUCCESS);

    result = vkQueueWaitIdle(queue);
    assert(result == VkResult::VK_SUCCESS);
}