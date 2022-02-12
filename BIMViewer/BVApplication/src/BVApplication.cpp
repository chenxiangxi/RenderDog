#include "BVApplication.h"
#include <QApplication>
#include <QHBoxLayout>
#include "MainWindow.h"
#include "Canvas.h"

BVApplication::BVApplication(int& argc, char** argv) : 
	m_QApplication(new QApplication(argc, argv)), 
	m_mainWindow(new MainWindow()){
	initApplication();
	initMainWindow();
}

BVApplication::~BVApplication() {
	delete m_mainWindow;
	delete m_QApplication;
}

void BVApplication::initApplication() {
	m_QApplication->setApplicationDisplayName("BIM Viewer");
}

void BVApplication::initMainWindow() {
	m_canvas = new Canvas();

	auto layout = new QHBoxLayout();
	layout->addWidget(m_canvas);
	layout->setMargin(0);

	auto centralWidget = new QWidget();
	centralWidget->setLayout(layout);

	m_mainWindow->setCentralWidget(centralWidget);
	m_mainWindow->resize(1024, 768);
}

int BVApplication::run() {
	m_mainWindow->show();
	return m_QApplication->exec();
}