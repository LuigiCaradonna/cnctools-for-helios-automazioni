#include "CncTools.h"
#include <QtWidgets/QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CncTools w;
    w.setWindowState(Qt::WindowMaximized);
    w.setStyle(QStyleFactory::create("WindowsVista"));
    w.show();
    return a.exec();
}
