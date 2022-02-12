#include "VulkanApplication.h"
#include "VulkanUtils.h"


static std::string AppName = "RenderDog";
static uint32_t AppVersion = 1;
static std::string EngineName = "Vulkan";
static uint32_t EngineVersion = 1;

void VulkanApplication::init(const std::vector<const char*>& requiredInstanceExtensionNames, const std::vector<const char*>& requiredInstanceLayerNames,
    const std::vector<const char*>& requiredDeviceExtensionNames, void* hwnd) {
    try
    {
        m_requiredInstanceExtensionNames = requiredInstanceExtensionNames;
        m_requiredInstanceLayerNames = requiredInstanceLayerNames;
        m_requiredDeviceExtensionNames = requiredDeviceExtensionNames;

        createInstance(hwnd);
    }
    catch (vk::SystemError& err)
    {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        throw err;
    }
}

void VulkanApplication::render() {

}

void VulkanApplication::destroy() {
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanApplication::createInstance(void* hwnd) {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = AppName.c_str();
    appInfo.applicationVersion = AppVersion;
    appInfo.pEngineName = EngineName.c_str();
    appInfo.engineVersion = EngineVersion;
    appInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo instanceCreateInfo = {};
    instanceCreateInfo.pApplicationInfo = &appInfo;

    m_instanceExtensionNames = getInstanceExtensionNames(m_requiredInstanceExtensionNames);
    instanceCreateInfo.enabledExtensionCount = m_instanceExtensionNames.size();
    instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensionNames.data();

    m_instanceLayerNames = getInstanceLayerNames(m_requiredInstanceLayerNames);
    instanceCreateInfo.enabledLayerCount = m_instanceLayerNames.size();
    instanceCreateInfo.ppEnabledLayerNames = m_instanceLayerNames.data();
    auto result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);

    enumerateSuitablePhysicalDevices(hwnd);
    createDeviceDefault(hwnd);
}

void VulkanApplication::createDeviceDefault(void* hwnd) {
    if (m_devices.size() == 0) throw "Cannot find device";

    m_currentDevice = VulkanDevice::findOpticalDevice(m_devices);
    m_currentDevice->init(m_requiredDeviceExtensionNames, hwnd);
}

void VulkanApplication::enumerateSuitablePhysicalDevices(void* hwnd) {
    uint32_t count;
    vkEnumeratePhysicalDevices(m_instance, &count, nullptr);

    std::vector<VkPhysicalDevice> physicalDevices(count);
    vkEnumeratePhysicalDevices(m_instance, &count, physicalDevices.data());

    std::for_each(physicalDevices.begin(), physicalDevices.end(), [&](VkPhysicalDevice& physicalDevice) {
        uint32_t count;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, nullptr);
        std::vector<VkExtensionProperties> extensionProperties(count);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &count, extensionProperties.data());
        auto availableExtensionProperties = filterExtensionNames(extensionProperties, m_requiredDeviceExtensionNames);
        if (availableExtensionProperties.size() != m_requiredDeviceExtensionNames.size()) return;

        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);
        if (properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
            properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU &&
            properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) return;

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(physicalDevice, &features);
        if (!features.geometryShader) return;

        m_devices.push_back(std::make_shared<VulkanDevice>(m_instance, physicalDevice));
        });
}

std::vector<const char*> VulkanApplication::filterExtensionNames(const std::vector<VkExtensionProperties>& properties, const std::vector<const char*>& requiredNames) {
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
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    if (count == 0) return std::vector<const char*>();

    std::vector<VkExtensionProperties> properties(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.data());

    return VulkanUtils::filterExtensionNames(properties, requiredNames);
}

std::vector<const char*> VulkanApplication::filterLayerNames(const std::vector<VkLayerProperties>& properties, const std::vector<const char*>& requiredNames) {
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
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    if (0 == count) return std::vector<const char*>();

    std::vector<VkLayerProperties> properties(count);
    vkEnumerateInstanceLayerProperties(&count, properties.data());

    return VulkanUtils::filterLayerNames(properties, requiredNames);
}