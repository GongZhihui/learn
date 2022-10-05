#include <QTimer>

#include "d3d11widget.h"
#include "graphics.h"

class D3d11Widget::Private 
{
public:
    Private(D3d11Widget& w)
        : w_{ w }
        , gfx_{ MakeGraphics(reinterpret_cast<HWND>(w.winId())) }
        , timer_{ new QTimer }
    {
        timer_->setInterval(1000 / 60);
        QObject::connect(timer_.get(), &QTimer::timeout, [this]() {
            render();
        });
        timer_->start();

    }

    void render() 
    {
        gfx_->render();
    }

private:
    QScopedPointer<QTimer> timer_;
    D3d11Widget& w_;
    Graphics::Pointer gfx_;
};

D3d11Widget::D3d11Widget(QWidget* parent)
    : QWidget{ parent }
    , MAKE_PRIVATRE(D3d11Widget, *this)
{
    setAttribute(Qt::WA_PaintOnScreen, true);
}

D3d11Widget::~D3d11Widget()
{
}

QPaintEngine* D3d11Widget::paintEngine() const
{
    return nullptr;
}
