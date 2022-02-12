#include "VulkanDevice.h"
#include "VulkanUtils.h"
#include <assert.h>

VulkanDevice::VulkanDevice(const VkInstance& instance, const VkPhysicalDevice& physicalDevice) : m_instance(instance), m_physicalDevice(physicalDevice) {
}

void VulkanDevice::findQueueFamily() {
    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, nullptr);

    std::vector<VkQueueFamilyProperties> properties(count);
    vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, properties.data());

    for (uint32_t i = 0; i < count; i++) {
        if(properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && !m_graphicQueueFamilyIndex.has_value()) {
            m_graphicQueueFamilyIndex = i;
        }
        if (m_surface && !m_presentQueueFamilyIndex.has_value()) {
            VkBool32 presentSupport = false;
            VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);
            if (result == VK_SUCCESS && presentSupport) {
                m_presentQueueFamilyIndex = i;
            }
        }
    }
}

void VulkanDevice::init(const std::vector<const char*>& requiredDeviceExtensionNames, void* hwnd) {
    if (m_inited) return;
    m_inited = true;

    createSurface(hwnd);

    findQueueFamily();

    createDevice(requiredDeviceExtensionNames);

    vkGetDeviceQueue(m_device, m_graphicQueueFamilyIndex.value(), 0, &m_queue);

    createSwapchain();
}

void VulkanDevice::createDevice(const std::vector<const char*>& requiredDeviceExtensionNames) {
    VkDeviceCreateInfo deviceCreateInfo = {};
    m_deviceExtensionNames = getDeviceExtensionNames(requiredDeviceExtensionNames);
    deviceCreateInfo.enabledExtensionCount = (uint32_t)m_deviceExtensionNames.size();
    deviceCreateInfo.ppEnabledExtensionNames = m_deviceExtensionNames.data();

    float queuePriorities[1] = { 0.0 };
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    std::set<uint32_t> queueFamilyIndices;
    if (m_graphicQueueFamilyIndex.has_value()) queueFamilyIndices.insert(m_graphicQueueFamilyIndex.value());
    if (m_presentQueueFamilyIndex.has_value()) queueFamilyIndices.insert(m_presentQueueFamilyIndex.value());
    std::for_each(queueFamilyIndices.begin(), queueFamilyIndices.end(), [&](uint32_t queueFamilyIndex) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = queuePriorities;
        queueCreateInfos.push_back(queueCreateInfo);
        });

    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.queueCreateInfoCount = queueCreateInfos.size();

    VkResult result = vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);
    assert(result == VK_SUCCESS);
}

std::vector<const char*> VulkanDevice::getDeviceExtensionNames(const std::vector<const char*>& requiredNames) {
    uint32_t count;
    VkResult result = vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &count, nullptr);
    if (result != VK_SUCCESS || 0 == count) return std::vector<const char*>();

    std::vector<VkExtensionProperties> properties(count);
    result = vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &count, properties.data());
    if (result != VK_SUCCESS)
        throw "Enumerate device extension properties failed";

    return VulkanUtils::filterExtensionNames(properties, requiredNames);
}

VkResult VulkanDevice::createCommandBuffer() {
    VkCommandPoolCreateInfo commandPoolInfo = {};
    commandPoolInfo.queueFamilyIndex = m_graphicQueueFamilyIndex.value();
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    VkResult result = vkCreateCommandPool(m_device, &commandPoolInfo, nullptr, &m_commandPool);
    assert(result == VK_SUCCESS);

    return result;
}

void VulkanDevice::createSurface(void* hwnd) {
    if (!hwnd) return;

#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
    surfaceCreateInfo.hwnd = reinterpret_cast<HWND>(hwnd);

    VkResult result = vkCreateWin32SurfaceKHR(m_instance, &surfaceCreateInfo, nullptr, &m_surface);
    assert(result == VK_SUCCESS);
    if (result != VK_SUCCESS) {
        throw "Cannot create surface";
    }
#endif

    uint32_t formatCount;
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr);
    assert(result == VK_SUCCESS);
    m_surfaceSupportedFormats.clear();
    m_surfaceSupportedFormats.resize(formatCount);
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, m_surfaceSupportedFormats.data());
    assert(result == VK_SUCCESS);
    if (formatCount == 1 && m_surfaceSupportedFormats[0].format == VK_FORMAT_UNDEFINED) {
        m_surfaceFormat = VK_FORMAT_R8G8B8A8_UNORM;
    }
    else {
        m_surfaceFormat = m_surfaceSupportedFormats[0].format;
    }
}

void VulkanDevice::createSwapchain() {
    if (!m_surface || !m_presentQueueFamilyIndex.has_value()) return;

    VkSwapchainKHR oldSwapchain = m_swapchain;

    VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.oldSwapchain = oldSwapchain;
    swapchainCreateInfo.surface = m_surface;
    swapchainCreateInfo.imageFormat = m_surfaceFormat;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageExtent.width = m_surfaceWidth;
    swapchainCreateInfo.imageExtent.height = m_surfaceHeight;
    swapchainCreateInfo.clipped = true;
    swapchainCreateInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    if (m_graphicQueueFamilyIndex != m_presentQueueFamilyIndex) {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        std::vector<uint32_t> queueFamilyIndices = { m_graphicQueueFamilyIndex.value(), m_presentQueueFamilyIndex.value() };
        swapchainCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }
    else {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    }


    VkResult result = vkCreateSwapchainKHR(m_device, &swapchainCreateInfo, nullptr, &m_swapchain);
    assert(result == VK_SUCCESS);

    uint32_t imageCount;
    result = vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, nullptr);
    assert(result == VK_SUCCESS && imageCount > 0);

    m_swapchainImageViews.clear();

    m_swapchainImages.clear();
    m_swapchainImages.resize(imageCount);

    result = vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, m_swapchainImages.data());
    assert(result == VK_SUCCESS);

    if (oldSwapchain) {
        vkDestroySwapchainKHR(m_device, oldSwapchain, nullptr);
    }

    m_swapchainImageViews.clear();
    for (uint32_t i = 0; i < m_swapchainImages.size(); ++i) {
        VkImageViewCreateInfo imageViewCreateInfo = {};
        imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewCreateInfo.image = m_swapchainImages[i];
        imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY };
        imageViewCreateInfo.format = m_surfaceFormat;
        imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
        imageViewCreateInfo.subresourceRange.levelCount = 1;
        imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        imageViewCreateInfo.subresourceRange.layerCount = 1;
        imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

        VkImageView imageView;
        result = vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &imageView);
        assert(result == VK_SUCCESS);
        m_swapchainImageViews.push_back(imageView);
    }

    //createDepthImage();

}

void VulkanDevice::createDepthImage() {
    VkImageCreateInfo imageCreateInfo = {};

    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(m_physicalDevice, m_depthImage.format, &formatProperties);
    if (formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
        imageCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
    }
    else if (formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
        imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    }
    else {
        throw "Unsupport depth format";
    }

    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = m_depthImage.format;
    imageCreateInfo.extent.width = m_surfaceWidth;
    imageCreateInfo.extent.height = m_surfaceHeight;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    if (m_graphicQueueFamilyIndex != m_presentQueueFamilyIndex) {
        imageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
        std::vector<uint32_t> queueFamilyIndices = { m_graphicQueueFamilyIndex.value(), m_presentQueueFamilyIndex.value() };
        imageCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
        imageCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
    }
    else {
        imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageCreateInfo.queueFamilyIndexCount = 0;
        imageCreateInfo.pQueueFamilyIndices = nullptr;
    }

    
    VkResult result = vkCreateImage(m_device, &imageCreateInfo, nullptr, &m_depthImage.image);
    assert(result == VK_SUCCESS);

    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(m_device, m_depthImage.image, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo = {};
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.memoryTypeIndex = 0;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;
    
    result = vkAllocateMemory(m_device, &memoryAllocateInfo, nullptr, &m_depthImage.memory);
    assert(result == VK_SUCCESS);

    result = vkBindImageMemory(m_device, m_depthImage.image, m_depthImage.memory, 0);
    assert(result == VK_SUCCESS);

    VkImageViewCreateInfo imageViewCreateInfo = {};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.image = VK_NULL_HANDLE;
    imageViewCreateInfo.format = m_depthImage.format;
    imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY };
    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    if (m_depthImage.format == VK_FORMAT_D16_UNORM_S8_UINT ||
        m_depthImage.format == VK_FORMAT_D24_UNORM_S8_UINT ||
        m_depthImage.format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
        imageViewCreateInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    VkCommandBufferAllocateInfo cmdBufferAllocatorInfo = {};
    cmdBufferAllocatorInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmdBufferAllocatorInfo.pNext = NULL;
    cmdBufferAllocatorInfo.commandPool = m_commandPool;
    cmdBufferAllocatorInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmdBufferAllocatorInfo.commandBufferCount = 1;
    vkAllocateCommandBuffers(m_device, &cmdBufferAllocatorInfo, &m_depthImageCmdBuffer);
    //vkBeginCommandBuffer(m_depthImageCmdBuffer, &cmdBufferBeginInfo);
    //vkEndCommandBuffer(m_depthImageCmdBuffer);
}

std::shared_ptr<VulkanDevice> VulkanDevice::findOpticalDevice(const std::vector<std::shared_ptr<VulkanDevice>>& devices) {
    uint32_t result = 0;
    auto optimalPhysicalDevice = devices[result]->getPhysicalDevice();
    VkPhysicalDeviceProperties optimalProperties = {};
    VkPhysicalDeviceMemoryProperties optimalMemProperties = {};
    vkGetPhysicalDeviceProperties(optimalPhysicalDevice, &optimalProperties);
    vkGetPhysicalDeviceMemoryProperties(optimalPhysicalDevice, &optimalMemProperties);

    for (auto i = 1; i < devices.size(); ++i) {
        optimalPhysicalDevice = devices[i]->getPhysicalDevice();
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(optimalPhysicalDevice, &properties);
        VkPhysicalDeviceMemoryProperties memProperties = {};
        vkGetPhysicalDeviceMemoryProperties(optimalPhysicalDevice, &memProperties);

        if (VulkanUtils::cmpDeviceType(properties.deviceType, optimalProperties.deviceType) < 0 ||
            memProperties.memoryHeapCount > optimalMemProperties.memoryHeapCount) {
            optimalProperties = properties;
            optimalMemProperties = memProperties;
            result = i;
            continue;
        }
    }

    return devices[result];
}