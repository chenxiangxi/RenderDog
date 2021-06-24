#include "VulkanApplication.h"

static std::string AppName = "RenderDog";
static uint32_t AppVersion = 1;
static std::string EngineName = "Vulkan";
static uint32_t EngineVersion = 1;

void VulkanApplication::init() {
    try
    {
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

    auto extensionNames = updateInstanceExtensionProperties();
    instanceCreateInfo.setEnabledExtensionCount(extensionNames.size());
    instanceCreateInfo.setPpEnabledExtensionNames(extensionNames.data());

    auto layerNames = updateInstanceLayerProperties();
    instanceCreateInfo.setEnabledLayerCount(layerNames.size());
    instanceCreateInfo.setPpEnabledLayerNames(layerNames.data());
    m_instance = vk::createInstance(instanceCreateInfo);
}

std::vector<const char*> VulkanApplication::updateInstanceExtensionProperties() {
    std::vector<const char*> requiredProperties = {
        VK_KHR_SURFACE_EXTENSION_NAME
    };
    return getInstanceExtensionProperties(requiredProperties);
}

std::vector<const char*> VulkanApplication::getInstanceExtensionProperties(const std::vector<const char*> requiredProperties) {
    uint32_t count = 0;
    vk::enumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    if (count == 0) return std::vector<const char*>();

    std::vector<vk::ExtensionProperties> properties;
    properties.resize(count);
    vk::enumerateInstanceExtensionProperties(nullptr, &count, properties.data());

    std::vector<const char*> propertyNames;

    std::for_each(requiredProperties.begin(), requiredProperties.end(), [&](const char* extName) {
        for (int i = 0; i < count; ++i) {
            if (std::strcmp(extName, properties[i].extensionName) == 0) {
                propertyNames.push_back(extName);
                break;
            }
        }
     });

    return propertyNames;
}

std::vector<const char*> VulkanApplication::updateInstanceLayerProperties() {
    std::vector<const char*> requiredLayerNames = {
        "VK_LAYER_LUNARG_api_dump"
    };
    return getInstanceLayerProperties(requiredLayerNames);
}

std::vector<const char*> VulkanApplication::getInstanceLayerProperties(const std::vector<const char*> requiredProperties) {
    uint32_t count;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    if (0 == count) return std::vector<const char*>();

    std::vector<VkLayerProperties> properties;
    properties.resize(count);
    vkEnumerateInstanceLayerProperties(&count, properties.data());

    std::vector<const char*> propertyNames;

    std::for_each(requiredProperties.begin(), requiredProperties.end(), [&](const char* layerName) {
        for (int i = 0; i < count; ++i) {
            if (std::strcmp(layerName, properties[i].layerName) == 0) {
                propertyNames.push_back(layerName);
                break;
            }
        }
        });

    return propertyNames;
}