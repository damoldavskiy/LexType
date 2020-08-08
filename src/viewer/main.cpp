#include <QApplication>
#include <QFontDatabase>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/Lato-Regular.ttf");
    a.setFont(QFont("Lato", 11));

    Viewer::MainWindow w;
    w.show();

    return a.exec();
}
