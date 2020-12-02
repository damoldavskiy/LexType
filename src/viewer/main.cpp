#include <QApplication>
#include <QFontDatabase>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/Lato-Regular.ttf");
    a.setFont(QFont("Lato", 11));

    QString path;
    if (argc > 1)
        path = argv[1];

    Viewer::MainWindow w(0, path);
    w.show();

    return a.exec();
}
