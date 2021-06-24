#include "VulkanRHI.h"
#include "VulkanApplication.h"

VulkanRHI::~VulkanRHI() {
    delete m_Application;
}

void VulkanRHI::init() {
    try
    {
        m_Application = new VulkanApplication();
        m_Application->init();
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

void VulkanRHI::destroy() {
    m_Application->destroy();
}

