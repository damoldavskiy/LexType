#include <QtTest>

#include "tst_text.h"

#define RUN(test) \
    status |= QTest::qExec(new test(), argc, argv)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int status = 0;

    RUN(tst_Text);

    return status;
}
