#include <QApplication>
#include <QFontDatabase>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qRegisterMetaTypeStreamOperators<SnippetManager>("SnippetManager");
    qRegisterMetaTypeStreamOperators<Snippet>("Snippet");

    QApplication a(argc, argv);
    QFontDatabase::addApplicationFont(":/Lato-Regular.ttf");
    QFontDatabase::addApplicationFont(":/CMUSerif-Roman.ttf");

    a.setFont(QFont("Lato", 11));

    QString path;
    if (argc > 1)
        path = argv[1];

    MainWindow w(0, path);
    w.show();

    return a.exec();
}
