#include <QString>
#include <QtTest>

#include "../src/text.h"

class tst_Text : public QObject
{
    Q_OBJECT

private slots:
    void basicTestCase();
    void oneLineBenchmark();
    void multiLineBenchmark();
};

void tst_Text::basicTestCase()
{
    QFont font;
    Text text(font);

    QCOMPARE(text.width(), 0.0);
    QCOMPARE(text.size(), 0);
    QCOMPARE(text.lineCount(), 1);

    text.insert(0, "Hello, world!\nThere is something");

    QVERIFY(text.width() > 0);
    QCOMPARE(text.size(), 32);
    QCOMPARE(text.lineCount(), 2);

    text.insert(0, "\n\n");

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

void tst_Text::oneLineBenchmark()
{
    QFont font;
    Text text(font);

    QBENCHMARK {
        text.insert(0, "Simple text with length of 30!");
    }
}

void tst_Text::multiLineBenchmark()
{
    QFont font;
    Text text(font);

    QBENCHMARK {
        text.insert(0, "Simple line with length of 30\n");
    }
}

QTEST_MAIN(tst_Text)

#include "tst_text.moc"
