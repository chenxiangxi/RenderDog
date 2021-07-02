#include "Common.h"


class QApplication;
class MainWindow;
class Canvas;

class LIBRARY_API BVApplication {
public:
	BVApplication(int& argc, char** argv);
	~BVApplication();
	int run();

private:
	void initApplication();
	void initMainWindow();

	QApplication* m_QApplication;
	MainWindow* m_mainWindow;
	Canvas* m_canvas;
};