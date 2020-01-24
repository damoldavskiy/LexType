#include <QtTest>

#include "../../src/editor/mathwriter.h"

class tst_MathWriter : public QObject
{
    Q_OBJECT

private slots:
    inline void basic();
    inline void fractions();
};

void tst_MathWriter::basic()
{
    QCOMPARE(MathWriter::apply("f: x -> y"), QString("f \\colon x \\to y"));
    QCOMPARE(MathWriter::apply("f : x > y"), QString("f : x > y"));
}

void tst_MathWriter::fractions()
{
    QCOMPARE(MathWriter::applyFractions(" 1/2 " ), QString(" \\frac{1}{2} "));
    QCOMPARE(MathWriter::applyFractions(" -1/2 "), QString(" -\\frac{1}{2} "));
    QCOMPARE(MathWriter::applyFractions(" 1234/2 "), QString(" \\frac{1234}{2} "));
    QCOMPARE(MathWriter::applyFractions(" 1/123 "), QString(" \\frac{1}{123} "));
    QCOMPARE(MathWriter::applyFractions(" (x + 2)/(2 + x) "), QString(" \\frac{x + 2}{2 + x} "));
    QCOMPARE(MathWriter::applyFractions(" (1/2) "), QString(" (\\frac{1}{2}) "));
    QCOMPARE(MathWriter::applyFractions(" (sin(x)/2) "), QString(" (\\frac{sin(x)}{2}) "));
    QCOMPARE(MathWriter::applyFractions(" (exp(-x^2_m/2)) "), QString(" (exp(-\\frac{x^2_m}{2})) "));
}
