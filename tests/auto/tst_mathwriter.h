#include <QtTest>

#include "../../src/editor/mathwriter.h"

class tst_MathWriter : public QObject
{
    Q_OBJECT

private slots:
    inline void basic();
    inline void fractions();
    inline void matrices();
};

void tst_MathWriter::basic()
{
    auto dict = MathWriter::snippetList();
    QCOMPARE(MathWriter::apply("f: x → y", dict), QString("f \\colon x \\to y"));
    QCOMPARE(MathWriter::apply("f : x > y", dict), QString("f : x > y"));
}

void tst_MathWriter::fractions()
{
    QCOMPARE(MathWriter::applyFractions("1/2" ), QString("\\frac{1}{2}"));
    QCOMPARE(MathWriter::applyFractions("-1/2"), QString("-\\frac{1}{2}"));
    QCOMPARE(MathWriter::applyFractions("1234/2"), QString("\\frac{1234}{2}"));
    QCOMPARE(MathWriter::applyFractions("1/123"), QString("\\frac{1}{123}"));
    QCOMPARE(MathWriter::applyFractions("(x + 2)/(2 + x)"), QString("\\frac{x + 2}{2 + x}"));
    QCOMPARE(MathWriter::applyFractions("(1/2)"), QString("(\\frac{1}{2})"));
    QCOMPARE(MathWriter::applyFractions("(sin(x)/2)"), QString("(\\frac{sin(x)}{2})"));
    QCOMPARE(MathWriter::applyFractions("(exp(-x^2_m/2))"), QString("(exp(-\\frac{x^2_m}{2}))"));
    QCOMPARE(MathWriter::applyFractions("\\sqrt{x + 3}/sin(x + 3)"), QString("\\frac{\\sqrt{x + 3}}{sin(x + 3)}"));
    QCOMPARE(MathWriter::applyFractions("(sin(x) + e^2x)/y"), QString("\\frac{sin(x) + e^2x}{y}"));
    QCOMPARE(MathWriter::applyFractions("((2)/x)/(3/(y))"), QString("\\frac{\\frac{2}{x}}{\\frac{3}{y}}"));
}

void tst_MathWriter::matrices()
{
    QCOMPARE(MathWriter::applyMatrices("\\(1, 2; 3, 4)"), QString("\\begin{pmatrix} 1 & 2 \\\\ 3 & 4 \\end{pmatrix}"));
    QCOMPARE(MathWriter::applyMatrices("{{\\(1, 2), x > 0; 0, x = 0}}"), QString("\\begin{cases} \\begin{pmatrix} 1 & 2 \\end{pmatrix} ,& x > 0 \\\\ 0 ,& x = 0 \\end{cases}"));
    QCOMPARE(MathWriter::applyMatrices("\\({{1, 2; 2, 3}}, 3; 4)"), QString("\\begin{pmatrix} \\begin{cases} 1 ,& 2 \\\\ 2 ,& 3 \\end{cases} & 3 \\\\ 4 \\end{pmatrix}"));
}
