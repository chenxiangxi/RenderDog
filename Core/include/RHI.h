#pragma once
#include "Common.h"

#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#endif

class LIBRARY_API RHI {
public:
	RHI();

	virtual void init(void* hwnd = nullptr) = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;
};