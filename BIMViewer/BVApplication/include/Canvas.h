#include <QWindow>
#include "Renderer.h"

class Canvas : public QWindow {
public:
    ~Canvas();
protected:
    void showEvent(QShowEvent* event);

private:
    void init();

    Renderer* m_renderer;
};