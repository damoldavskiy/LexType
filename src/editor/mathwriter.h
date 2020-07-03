#ifndef MATHWRITER_H
#define MATHWRITER_H

#include <QVector>
#include <QString>
#include <QStack>

bool isDelimeter(QChar symbol);
bool isOpenBrace(QChar symbol, int direction = 1);
bool isCloseBrace(QChar symbol, int direction = 1);
bool isClosing(QChar open, QChar close, int direction = 1);

template <typename T>
int findBracePlace(const T &source, int start, int direction)
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

template <typename T>
int findBrace(const T &source, int start, int direction = 1)
{
    Q_ASSERT(isOpenBrace(source[start]) || isOpenBrace(source[start], -1));

    QStack<QChar> braces;
    for (int i = start; i >= 0 && i < source.size(); i += direction) {
        if (isOpenBrace(source[i], direction))
            braces.push(source[i]);
        else if (isCloseBrace(source[i], direction)) {
            if (braces.size() > 0 && isClosing(braces.top(), source[i], direction))
                braces.pop();
            else // Brace sequence is incorrect
                return -1;
        }

        if (braces.size() == 0)
            return i;
    }

    // Close brace not found
    return -1;
}

class MathWriter
{
public:
    static QString pass(const QString &source);
    static QString applyParameters(QString source);
    static QString applyMatrices(QString source);
    static QString applyFractions(QString source);
    static QString apply(QString source);
};

#endif // MATHWRITER_H
