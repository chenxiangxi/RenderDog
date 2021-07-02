#pragma once
#include "Common.h"
#include "RHI.h"

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#endif

class VulkanApplication;

class LIBRARY_API VulkanRHI : public RHI {
public:
	VulkanRHI();
	~VulkanRHI();
	virtual void init();
	virtual void createSurface(HWND hInstance);
	virtual void render();
	virtual void destroy();

private:
	VulkanApplication* m_Application;
};