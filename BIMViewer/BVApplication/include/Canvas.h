#include <QWidget>
#include "Renderer.h"

class Canvas : public QWidget {
public:
    ~Canvas();
protected:
    void showEvent(QShowEvent* event);
    void paintEvent(QPaintEvent* event);

private:
    void init();

    Renderer* m_renderer;
};