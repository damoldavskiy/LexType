#include <QCoreApplication>
#include <QTimer>

#include "program.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Program p;
    QObject::connect(&p, &Program::finished, &a, &QCoreApplication::quit);
    QTimer::singleShot(0, &p, &Program::run);

    return a.exec();
}
