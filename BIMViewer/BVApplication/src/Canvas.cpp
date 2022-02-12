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
	m_renderer->getRHI()->init((HWND)this->winId());
}

void Canvas::showEvent(QShowEvent* event) {
	QWidget::showEvent(event);
	init();
}

void Canvas::paintEvent(QPaintEvent* event) {
	m_renderer->getRHI()->render();
}