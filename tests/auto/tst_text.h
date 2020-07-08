#include <QtTest>

#include "../../src/editor/text.h"

class tst_Text : public QObject
{
    Q_OBJECT

private slots:
    inline void basic();
    inline void removeInline();
    inline void removeOutline();
};

void tst_Text::basic()
{
    QFont font;
    Text text(font, 4);

    QCOMPARE(text.width(), 0.0);
    QCOMPARE(text.size(), 0);
    QCOMPARE(text.lineCount(), 1);

    text.insert(0, "Hello, world!\nThere is something");
    QCOMPARE(text.text(), QString("Hello, world!\nThere is something"));

    QVERIFY(text.width() > 0);
    QCOMPARE(text.size(), 32);
    QCOMPARE(text.lineCount(), 2);

    text.insert(0, "\n\n");
    QCOMPARE(text.text(), QString("\n\nHello, world!\nThere is something"));

    QCOMPARE(text.lineCount(), 4);

    QCOMPARE(text.lineSize(0), 0);
    QCOMPARE(text.lineSize(1), 0);
    QCOMPARE(text.lineSize(2), 13);
    QCOMPARE(text.lineSize(3), 18);

    QCOMPARE(text.lineStart(0), 0);
    QCOMPARE(text.lineStart(1), 1);
    QCOMPARE(text.lineStart(2), 2);
    QCOMPARE(text.lineStart(3), 16);

    QCOMPARE(text.mid(5, 13), QString("lo, world!\nTh"));

    text.remove(0, 34);

    QCOMPARE(text.width(), 0.0);
    QCOMPARE(text.size(), 0);
    QCOMPARE(text.lineCount(), 1);
    QCOMPARE(text.lineSize(0), 0);
    QCOMPARE(text.lineStart(0), 0);
}

void tst_Text::removeInline()
{
    QFont font;
    Text text(font, 4);

    text.insert(0,"some\ttext\n");
    text.remove(1, 3);
    text.insert(1, "body");

    QCOMPARE(text.text(), QString("sbody\ttext\n"));
    QCOMPARE(text.lineCount(), 2);
    QCOMPARE(text.lineSize(0), 10);
    QCOMPARE(text.lineStart(0), 0);
    QCOMPARE(text.lineSize(1), 0);
    QCOMPARE(text.lineStart(1), 11);
}

void tst_Text::removeOutline()
{
    QFont font;
    Text text(font, 4);

    text.insert(0, "012345\n6789\n012345678\n");
    text.remove(4, 13);

    QCOMPARE(text.text(), QString("01235678\n"));
    QCOMPARE(text.size(), 9);
    QCOMPARE(text.lineCount(), 2);
    QCOMPARE(text.lineSize(0), 8);
    QCOMPARE(text.lineStart(0), 0);
    QCOMPARE(text.lineSize(1), 0);
    QCOMPARE(text.lineStart(1), 9);
}
