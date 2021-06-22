#include "VulkanRHI.h"
#include "vulkan/vulkan.hpp"

void VulkanRHI::Init() {
    try
    {
        // initialize the vk::ApplicationInfo structure
        vk::ApplicationInfo applicationInfo("test", 1, "test", 1, VK_API_VERSION_1_1);

        // initialize the vk::InstanceCreateInfo
        vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);

        // create an Instance
        vk::Instance instance = vk::createInstance(instanceCreateInfo);

        // destroy it again
        instance.destroy();
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