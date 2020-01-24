#include "mathwriter.h"

#include <QVector>

bool isDelimeter(QChar symbol)
{
    return symbol == ' ' || symbol == ',' || symbol == ';';
}

int findBrace(const QString &source, int start, int direction)
{
    QChar open = '(';
    QChar close = ')';
    if (direction == -1)
        qSwap(open, close);

    int i = start + direction;
    if (i < 0 || i >= source.size() || isDelimeter(source[i]))
        return  i - direction;

    int braces = 0;
    do {
        if (source[i] == open)
            ++braces;
        else if (source[i] == close)
            --braces;
        i += direction;
    } while (i >= 0 && i < source.size() && (braces > 0 || (braces == 0 && !isDelimeter(source[i]))));
    i -= direction;

    // (sin(x)/x) = (\frac{sin(x)}{x})
    if (source[i] == close && braces < 0)
        i -= direction;

    return i;
}

QString MathWriter::pass(const QString &source)
{
    QString result;
    bool math = false;
    int start;

    for (int i = 0; i < source.size(); ++i) {
        if (source[i] == '`') {
            math = !math;
            if (math)
                start = i + 1;
            else // TODO string_view
                result += "$" + apply(source.mid(start, i - start)) + "$";
        } else if (!math) {
            result += source[i];
        }
    }

    return result;
}

QString MathWriter::applyFractions(QString source)
{
    for (int i = 0; i < source.size(); ++i) {
        if (source[i] == '/') {
            int leftBrace = findBrace(source, i, -1);
            int rightBrace = findBrace(source, i, 1);

            if (leftBrace != i && rightBrace != i) {
                if (source[leftBrace] == '-')
                    ++leftBrace;
                if (source[i - 1] == ')' && source[leftBrace] == '(') {
                    source.remove(i - 1, 1);
                    source.remove(leftBrace, 1);
                    i -= 2;
                    rightBrace -= 2;
                }
                if (source[i + 1] == '(' && source[rightBrace] == ')') {
                    source.remove(rightBrace, 1);
                    source.remove(i + 1, 1);
                    rightBrace -= 2;
                }
                source.remove(i, 1);
                source.insert(rightBrace, "}");
                source.insert(i, "}{");
                source.insert(leftBrace, "\\frac{");
            }
        }
    }

    return source;
}

QString MathWriter::apply(QString source)
{
    QVector<QPair<QString, QString>> dict;

    dict.append({ "(", "\\left(" });
    dict.append({ ")", "\\right)" });

    dict.append({ "...", "\\dots" });
    dict.append({ "*", "\\cdot" });

    dict.append({ "sin", "\\sin" });
    dict.append({ "cos", "\\cos" });
    dict.append({ "sqrt", "\\sqrt" });

    dict.append({ "lim", "\\lim\\limits" });
    dict.append({ "sum", "\\sum\\limits" });

    dict.append({ "<=>", "\\iff" });
    dict.append({ "<->", "\\leftrightarrow" });
    dict.append({ "=>", "\\implies" });
    dict.append({ "<=", "\\impliedby" });
    dict.append({ "->", "\\to" });
    dict.append({ "<-", "\\leftarrow" });

    dict.append({ "forall", "\\forall" });
    dict.append({ "in", "\\in" });
    dict.append({ "exists", "\\exists" });
    dict.append({ "not", "\\not" });

    dict.append({ "!=", "\\ne" });
    dict.append({ "+-", "\\pm" });
    dict.append({ "~=", "\\approx" });

    dict.append({ "\\N", "\\mathbb{N}" });
    dict.append({ "\\R", "\\mathbb{R}" });
    dict.append({ "\\C", "\\mathbb{C}" });

    dict.append({ "alpha", "\\alpha" });
    dict.append({ "Alpha", "\\Alpha" });
    dict.append({ "beta", "\\beta" });
    dict.append({ "Beta", "\\Beta" });
    dict.append({ "sigma", "\\sigma" });
    dict.append({ "Sigma", "\\Sigma" });

    // TODO More effective
    source = applyFractions(source);

    QString result;

    for (int i = 0; i < source.size(); ++i) {
        bool next = false;
        for (const auto &pair : dict)
            if (i + pair.first.size() <= source.size() && source.mid(i, pair.first.size()) == pair.first) {
                result += pair.second;
                i += pair.first.size() - 1;
                next = true;
                break;
            }
        if (next)
            continue;

        if (source[i] == ':' && i > 0 && !isDelimeter(source[i - 1])) {
            result += " \\colon";
            continue;
        }

        result += source[i];
    }

    return result;
}

