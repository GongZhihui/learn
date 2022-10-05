#include <iostream>
#include <QApplication>
#include <ui/d3d11widget.h>

int main(int argc, char** argv)
{
    QApplication app{ argc, argv };
    D3d11Widget w;
    w.resize(400, 300);
    w.show();
    

    return app.exec();
}