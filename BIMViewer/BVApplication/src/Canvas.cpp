#include "Canvas.h"
#include "VulkanRHI.h"

Canvas::~Canvas() {
	if (m_renderer) {
		delete m_renderer;
	}
}

void Canvas::init() {
	if (m_renderer) return;

	m_renderer = new Renderer(new VulkanRHI());
	m_renderer->getRHI()->init();
	m_renderer->getRHI()->createSurface((HWND)this->winId());
}

void Canvas::showEvent(QShowEvent* event) {
	QWindow::showEvent(event);
	init();
}