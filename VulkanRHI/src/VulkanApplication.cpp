#include "VulkanApplication.h"

static std::string AppName = "RenderDog";
static uint32_t AppVersion = 1;
static std::string EngineName = "Vulkan.hpp";
static uint32_t EngineVersion = 1;

void VulkanApplication::init() {
    try
    {
        createInstance();
    }
    catch (vk::SystemError& err)
    {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    }
    catch (std::exception& err)
    {
        std::cout << "std::exception: " << err.what() << std::endl;
        exit(-1);
    }
    catch (...)
    {
        std::cout << "unknown error\n";
        exit(-1);
    }
}

void VulkanApplication::destroy() {
    m_instance.destroy();
}

void VulkanApplication::createInstance() {
    vk::ApplicationInfo applicationInfo(AppName.c_str(), AppVersion, EngineName.c_str(), EngineVersion, VK_API_VERSION_1_1);

    std::vector<const char*> requiredExtensionProperties;
    requiredExtensionProperties.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
    std::vector<const char*> instanceExtensionProperties = getInstanceExtensionProperties(requiredExtensionProperties);

    vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);
    instanceCreateInfo.setEnabledExtensionCount(instanceExtensionProperties.size());
    instanceCreateInfo.setPpEnabledExtensionNames(instanceExtensionProperties.data());

    m_instance = vk::createInstance(instanceCreateInfo);
}

std::vector<const char*> VulkanApplication::getInstanceExtensionProperties(const std::vector<const char*> requiredExtensionProperties) {
    uint32_t count = 0;
    vk::enumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    if (count == 0) return std::vector<const char*>();

    vk::ExtensionProperties* properties = new vk::ExtensionProperties[count];
    vk::enumerateInstanceExtensionProperties(nullptr, &count, properties);

    std::vector<const char*> propertyNames;

    std::for_each(requiredExtensionProperties.begin(), requiredExtensionProperties.end(), [&](const char* extName) {
        for (int i = 0; i < count; ++i) {
            if (std::strcmp(extName, properties[i].extensionName) == 0) {
                propertyNames.push_back(properties[i].extensionName);
                break;
            }
        }
     });


    return propertyNames;
}

