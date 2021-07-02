#include "Renderer.h"

Renderer::Renderer(RHI* rhi) : m_rhi(rhi) {

}

Renderer::~Renderer() {
	if (m_rhi) {
		m_rhi->destroy();

		delete m_rhi;
	}
}