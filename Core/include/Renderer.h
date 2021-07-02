#pragma once
#include "Common.h"
#include "RHI.h"

class LIBRARY_API Renderer {
public:
	Renderer(RHI* rhi);
	~Renderer();

	RHI* getRHI() {
		return m_rhi;
	}
private:
	RHI* m_rhi;
};