#include "VulkanRHI.h"
#include "VulkanApplication.h"

VulkanRHI::VulkanRHI() {

}

VulkanRHI::~VulkanRHI() {
    delete m_Application;
}

void VulkanRHI::init(void* hwnd) {
    try{
        m_Application = new VulkanApplication();

        std::vector<const char*> requiredInstanceExtensionNames = { VK_KHR_SURFACE_EXTENSION_NAME };
        if (hwnd) {
#ifdef _WIN32
            requiredInstanceExtensionNames.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif
        }

#ifdef _DEBUG
        requiredInstanceExtensionNames.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

        std::vector<const char*> requiredInstanceLayerNames = { "VK_LAYER_LUNAGR_swapchain", "VK_LAYER_LUNARG_image", "VK_LAYER_KHRONOS_validation" };
#ifdef _DEBUG
        requiredInstanceLayerNames.push_back("VK_LAYER_LUNARG_api_dump");
#endif

        std::vector<const char*> requiredDeviceExtensionNames = {};
        if (hwnd) {
            requiredDeviceExtensionNames.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }

        m_Application->init(requiredInstanceExtensionNames, requiredInstanceLayerNames, requiredDeviceExtensionNames, hwnd);
    }
    catch (vk::SystemError& err){
        std::cout << "vkSystemError: " << err.what() << std::endl;
        exit(-1);
    }
    catch (std::exception& err){
        std::cout << "std::exception: " << err.what() << std::endl;
        exit(-1);
    }
    catch (...){
        std::cout << "unknown error\n";
        exit(-1);
    }
}

void VulkanRHI::render() {
    m_Application->render();
}

void VulkanRHI::destroy() {
    m_Application->destroy();
}
