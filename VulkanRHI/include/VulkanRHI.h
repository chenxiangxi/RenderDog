#pragma once
#include "Common.h"

class VulkanApplication;

class LIBRARY_API VulkanRHI {
public:
	~VulkanRHI();

	virtual void init();
	virtual void destroy();

private:
	VulkanApplication* m_Application;
};