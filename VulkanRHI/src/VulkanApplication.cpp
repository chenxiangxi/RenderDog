#include "VulkanApplication.h"
#include "VulkanUtils.h"


static std::string AppName = "RenderDog";
static uint32_t AppVersion = 1;
static std::string EngineName = "Vulkan";
static uint32_t EngineVersion = 1;

void VulkanApplication::init(const std::vector<const char*>& requiredInstanceExtensionNames,
    const std::vector<const char*>& requiredInstanceLayerNames,
    const std::vector<const char*>& requiredDeviceExtensionNames) {
    try
    {
        m_requiredInstanceExtensionNames = requiredInstanceExtensionNames;
        m_requiredInstanceLayerNames = requiredInstanceLayerNames;
        m_requiredDeviceExtensionNames = requiredDeviceExtensionNames;

        createInstance();
    }
    catch (vk::SystemError& err)
    {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        throw err;
    }
}

void VulkanApplication::destroy() {
    m_instance.destroy();
}

void VulkanApplication::createInstance() {
    vk::ApplicationInfo appInfo = vk::ApplicationInfo(AppName.c_str(), AppVersion, EngineName.c_str(), EngineVersion, VK_API_VERSION_1_1);
    vk::InstanceCreateInfo instanceCreateInfo({}, &appInfo);

    m_instanceExtensionNames = getInstanceExtensionNames(m_requiredInstanceExtensionNames);
    instanceCreateInfo.setEnabledExtensionCount(m_instanceExtensionNames.size());
    instanceCreateInfo.setPpEnabledExtensionNames(m_instanceExtensionNames.data());

    m_instanceLayerNames = getInstanceLayerNames(m_requiredInstanceLayerNames);
    instanceCreateInfo.setEnabledLayerCount(m_instanceLayerNames.size());
    instanceCreateInfo.setPpEnabledLayerNames(m_instanceLayerNames.data());
    m_instance = vk::createInstance(instanceCreateInfo);

    enumerateSuitablePhysicalDevices();
    createDeviceDefault();
}

void VulkanApplication::createDeviceDefault() {
    if (m_devices.size() == 0) throw "Cannot find device";

    vk::PhysicalDevice optimalPhysicalDevice;
    vk::PhysicalDeviceProperties optimalProperties;
    vk::PhysicalDeviceMemoryProperties optimalMemProperties;
    for (auto i = 0; i < m_devices.size(); ++i) {
        auto physicalDevice = m_devices[i].getPhysicalDevice();
        if (!optimalPhysicalDevice) {
            optimalPhysicalDevice = physicalDevice;
            physicalDevice.getProperties(&optimalProperties);
            physicalDevice.getMemoryProperties(&optimalMemProperties);
            m_currentDevice = m_devices[i];
            continue;
        }

        vk::PhysicalDeviceProperties properties;
        physicalDevice.getProperties(&properties);
        vk::PhysicalDeviceMemoryProperties memProperties;
        physicalDevice.getMemoryProperties(&memProperties);

        if (properties.deviceType < optimalProperties.deviceType || 
            memProperties.memoryHeapCount > optimalMemProperties.memoryHeapCount) {
            optimalPhysicalDevice = physicalDevice;
            optimalProperties = properties;
            optimalMemProperties = memProperties;
            m_currentDevice = m_devices[i];
            continue;
        }
    }

    m_currentDevice.init(m_requiredDeviceExtensionNames);
}



void VulkanApplication::enumerateSuitablePhysicalDevices() {
    uint32_t count;
    m_instance.enumeratePhysicalDevices(&count, nullptr);

    std::vector<vk::PhysicalDevice> physicalDevices(count);
    m_instance.enumeratePhysicalDevices(&count, physicalDevices.data());

    std::for_each(physicalDevices.begin(), physicalDevices.end(), [&](vk::PhysicalDevice& physicalDevice) {
        vk::PhysicalDeviceProperties properties;
        physicalDevice.getProperties(&properties);
        if (properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu && 
            properties.deviceType != vk::PhysicalDeviceType::eIntegratedGpu && 
            properties.deviceType != vk::PhysicalDeviceType::eVirtualGpu) return;

        vk::PhysicalDeviceFeatures features;
        physicalDevice.getFeatures(&features);
        if (!features.geometryShader) return;

        VulkanDevice vulkanDevice(physicalDevice);
        m_devices.push_back(vulkanDevice);
        });
}

std::vector<const char*> VulkanApplication::filterExtensionNames(const std::vector<vk::ExtensionProperties>& properties, const std::vector<const char*>& requiredNames) {
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

std::vector<const char*> VulkanApplication::getInstanceExtensionNames(const std::vector<const char*>& requiredNames) {
    uint32_t count = 0;
    vk::enumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    if (count == 0) return std::vector<const char*>();

    std::vector<vk::ExtensionProperties> properties(count);
    vk::enumerateInstanceExtensionProperties(nullptr, &count, properties.data());

    return VulkanUtils::filterExtensionNames(properties, requiredNames);
}

std::vector<const char*> VulkanApplication::filterLayerNames(const std::vector<vk::LayerProperties>& properties, const std::vector<const char*>& requiredNames) {
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

std::vector<const char*> VulkanApplication::getInstanceLayerNames(const std::vector<const char*>& requiredNames) {
    uint32_t count;
    vk::enumerateInstanceLayerProperties(&count, nullptr);
    if (0 == count) return std::vector<const char*>();

    std::vector<vk::LayerProperties> properties(count);
    vk::enumerateInstanceLayerProperties(&count, properties.data());

    return VulkanUtils::filterLayerNames(properties, requiredNames);
}