#include "AGV_Control.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    AGV_Control window;
    window.show();
    return app.exec();
}
