#include <QApplication>

#include "styler.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    qRegisterMetaTypeStreamOperators<SnippetManager>("SnippetManager");
    qRegisterMetaTypeStreamOperators<Snippet>("Snippet");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
