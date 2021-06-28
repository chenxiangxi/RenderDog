#include "VulkanUtils.h"


std::vector<const char*> VulkanUtils::filterExtensionNames(const std::vector<vk::ExtensionProperties>& properties, const std::vector<const char*>& requiredNames) {
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

std::vector<const char*> VulkanUtils::filterLayerNames(const std::vector<vk::LayerProperties>& properties, const std::vector<const char*>& requiredNames) {
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

int VulkanUtils::cmpDeviceType(vk::PhysicalDeviceType left, vk::PhysicalDeviceType right) {
    return 0;
}