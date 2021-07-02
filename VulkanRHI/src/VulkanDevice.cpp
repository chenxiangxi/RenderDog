#include "VulkanDevice.h"
#include "VulkanUtils.h"

VulkanDevice::VulkanDevice(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice) : m_instance(instance), m_physicalDevice(physicalDevice) {
    initQueueFamily();
}

void VulkanDevice::initQueueFamily() {
    uint32_t count;
    m_physicalDevice.getQueueFamilyProperties(&count, nullptr);

    std::vector<vk::QueueFamilyProperties> properties(count);
    m_physicalDevice.getQueueFamilyProperties(&count, properties.data());

    for (uint32_t i = 0; i < count; i++) {
        if(properties[i].queueFlags & vk::QueueFlagBits::eGraphics) {
            m_queueFamilyIndex = i;
            break;
        }
    }
}

void VulkanDevice::init(const std::vector<const char*>& requiredDeviceExtensionNames) {
    if (m_inited) return;
    m_inited = true;

    vk::DeviceCreateInfo deviceCreateInfo = {};
    m_deviceExtensionNames = getDeviceExtensionNames(requiredDeviceExtensionNames);
    deviceCreateInfo.setEnabledExtensionCount((uint32_t)m_deviceExtensionNames.size());
    deviceCreateInfo.setPpEnabledExtensionNames(m_deviceExtensionNames.data());

    float queuePriorities[1] = { 0.0 };
    vk::DeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.queueFamilyIndex = m_queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = queuePriorities;
    deviceCreateInfo.setPQueueCreateInfos(&queueCreateInfo);

    vk::Result result = m_physicalDevice.createDevice(&deviceCreateInfo, nullptr, &m_device);
}

std::vector<const char*> VulkanDevice::getDeviceExtensionNames(const std::vector<const char*>& requiredNames) {
    uint32_t count;
    m_physicalDevice.enumerateDeviceExtensionProperties(nullptr, &count, nullptr);
    if (0 == count) return std::vector<const char*>();

    std::vector<vk::ExtensionProperties> properties(count);
    m_physicalDevice.enumerateDeviceExtensionProperties(nullptr, &count, properties.data());

    return VulkanUtils::filterExtensionNames(properties, requiredNames);
}

void VulkanDevice::createCommandBuffer() {
    vk::CommandPoolCreateInfo commandPoolInfo = {};
    commandPoolInfo.queueFamilyIndex = m_queueFamilyIndex;
    commandPoolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
    m_device.createCommandPool(&commandPoolInfo, nullptr, &m_commandPool);
}

void VulkanDevice::createSurface(HWND hwnd) {
    vk::Win32SurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
    surfaceCreateInfo.hwnd = hwnd;

    m_instance.createWin32SurfaceKHR(&surfaceCreateInfo, nullptr, &m_surface);
}