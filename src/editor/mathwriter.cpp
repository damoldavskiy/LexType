#include "mathwriter.h"

#include <QVector>
#include <QStack>

QVector<QChar> delimeters = { ' ', '\n', ',', ';' };
QVector<QChar> openBraces = { '(', '{', '[' };
QVector<QChar> closeBraces = { ')', '}', ']' };

bool isDelimeter(QChar symbol)
{
    return delimeters.contains(symbol);
}

bool isOpenBrace(QChar symbol, int direction = 1)
{
    if (direction == 1)
        return openBraces.contains(symbol);
    else
        return closeBraces.contains(symbol);
}

bool isCloseBrace(QChar symbol, int direction = 1)
{
    if (direction == 1)
        return closeBraces.contains(symbol);
    else
        return openBraces.contains(symbol);
}

bool isClosing(QChar open, QChar close, int direction = 1)
{
    if (direction == 1)
        return openBraces.indexOf(open) != -1 && openBraces.indexOf(open) == closeBraces.indexOf(close);
    else
        return openBraces.indexOf(close) != -1 && openBraces.indexOf(close) == closeBraces.indexOf(open);
}

int findBracePlace(const QString &source, int start, int direction)
{
    int i = start + direction;
    if (i < 0 || i >= source.size() || isDelimeter(source[i]))
        return  i - direction;

    QStack<QChar> braces;
    bool bracesFail = false;
    do {
        if (isOpenBrace(source[i], direction))
            braces.push(source[i]);
        else if (isCloseBrace(source[i], direction)) {
            if (braces.size() > 0 && isClosing(braces.top(), source[i], direction))
                braces.pop();
            else
                bracesFail = true;
        }
        i += direction;
    } while (i >= 0 && i < source.size() && !bracesFail && (braces.size() > 0 || (braces.size() == 0 && !isDelimeter(source[i]))));
    i -= direction;

    // (sin(x)/x) -> (\frac{sin(x)}{x})
    if (isCloseBrace(source[i], direction) && bracesFail)
        i -= direction;

    return i;
}

int findBrace(const QString &source, int start, QChar open)
{
    QStack<QChar> braces;
    braces.append(open);

    for (int i = start; i < source.size(); ++i) {
        if (isOpenBrace(source[i]))
            braces.push(source[i]);
        else if (isCloseBrace(source[i])) {
            if (braces.size() > 0 && isClosing(braces.top(), source[i]))
                braces.pop();
            // TODO else brace sequence is incorrect
        }

        if (braces.size() == 0)
            return i;
    }

    return start;
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
            int leftBrace = findBracePlace(source, i, -1);
            int rightBrace = findBracePlace(source, i, 1);

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

QString MathWriter::applyParameters(QString source)
{
    // TODO \\sqrt(x + 2) -> \\sqrt{x + 2}, \\lim(x -> 0) -> \\lim_{x -> 0}
    return source;
}

QString MathWriter::apply(QString source)
{
    QVector<QPair<QString, QString>> dict;

    dict.append({ "(", "\\left(" });
    dict.append({ ")", "\\right)" });

    dict.append({ "...", "\\dots" });
    dict.append({ "***", "\\mdots" });
    dict.append({ "..-", "\\cdots" });
    dict.append({ "..|", "\\vdots" });
    dict.append({ "..\\", "\\ddots" });

    dict.append({ "**", "*" });
    dict.append({ "*", "\\cdot" });
    dict.append({ "@", "\\circ" });
    dict.append({ "o+", "\\oplus" });

    dict.append({ "sin", "\\sin" });
    dict.append({ "cos", "\\cos" });
    dict.append({ "sqrt", "\\sqrt" });
    dict.append({ "exp", "\\exp" });

    dict.append({ "lim", "\\lim" });
    dict.append({ "sum", "\\sum" });
    dict.append({ "oline", "\\overline" });
    dict.append({ "uline", "\\underline" });

    dict.append({ "matrix", "\\matrix" });
    dict.append({ "system", "\\system" });

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
    dict.append({ "~~", "\\approx" });
    dict.append({ "==", "\\equiv" });

    dict.append({ "le", "\\le" });
    dict.append({ "ge", "\\ge" });
    dict.append({ "ll", "\\ll" });
    dict.append({ "gg", "\\gg" });

    dict.append({ "<<", "\\langle" });
    dict.append({ ">>", "\\rangle" });

    dict.append({ "NN", "\\mathbb{N}" });
    dict.append({ "ZZ", "\\mathbb{Z}" });
    dict.append({ "RR", "\\mathbb{R}" });
    dict.append({ "CC", "\\mathbb{C}" });

    dict.append({ "!O", "\\varnothing" });

    dict.append({ "bar", "\\bar" });
    dict.append({ "hat", "\\hat" });

    dict.append({ "Alpha", "\\Alpha" });
    dict.append({ "alpha", "\\alpha" });
    dict.append({ "Beta", "\\Beta" });
    dict.append({ "beta", "\\beta" });
    dict.append({ "Gamma", "\\Gamma" });
    dict.append({ "gamma", "\\gamma" });
    dict.append({ "Delta", "\\Delta" });
    dict.append({ "delta", "\\delta" });
    dict.append({ "Epsilon", "\\Epsilon" });
    dict.append({ "epsilon", "\\epsilon" });
    dict.append({ "Zeta", "\\Zeta" });
    dict.append({ "zeta", "\\zeta" });
    dict.append({ "Eta", "\\Eta" });
    dict.append({ "eta", "\\eta" });
    dict.append({ "Theta", "\\Theta" });
    dict.append({ "theta", "\\theta" });
    dict.append({ "Iota", "\\Iota" });
    dict.append({ "iota", "\\iota" });
    dict.append({ "Kappa", "\\Kappa" });
    dict.append({ "kappa", "\\kappa" });
    dict.append({ "Lambda", "\\Lambda" });
    dict.append({ "lambda", "\\lambda" });
    dict.append({ "Mu", "\\Mu" });
    dict.append({ "mu", "\\mu" });
    dict.append({ "Nu", "\\Nu" });
    dict.append({ "nu", "\\nu" });
    dict.append({ "Xi", "\\Xi" });
    dict.append({ "xi", "\\xi" });
    dict.append({ "Omicron", "\\Omicron" });
    dict.append({ "omicron", "\\omicron" });
    dict.append({ "Pi", "\\Pi" });
    dict.append({ "pi", "\\pi" });

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

    result = applyParameters(result);

    return result;
}

