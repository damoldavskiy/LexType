#ifndef MATHWRITER_H
#define MATHWRITER_H

#include <QVector>
#include <QString>
#include <QStack>

bool isDelimeter(QChar symbol);
bool isOpenBracket(QChar symbol, int direction = 1);
bool isCloseBracket(QChar symbol, int direction = 1);
QChar getClosing(QChar open, int direction = 1);
bool isClosing(QChar open, QChar close, int direction = 1);

template <typename T>
int findBracketPlace(const T &source, int start, int direction = 1, QVector<QChar> extraDelimeters = { })
{
    int i = start + direction;
    if (i < 0 || i >= source.size() || isDelimeter(source[i]))
        return  i - direction;

    QStack<QChar> brackets;
    bool bracesFail = false;
    do {
        if (isOpenBracket(source[i], direction))
            brackets.push(source[i]);
        else if (isCloseBracket(source[i], direction)) {
            if (brackets.size() > 0 && isClosing(brackets.top(), source[i], direction))
                brackets.pop();
            else
                bracesFail = true;
        }
        i += direction;
    } while (i >= 0 && i < source.size() && !bracesFail && (brackets.size() > 0 || (brackets.size() == 0 && !isDelimeter(source[i]) && !extraDelimeters.contains(source[i]))));
    i -= direction;

    // (sin(x)/x) -> (\frac{sin(x)}{x})
    if (isCloseBracket(source[i], direction) && bracesFail)
        i -= direction;

    return i;
}

template <typename T>
int findBracket(const T &source, int start, int direction = 1, QChar first = QChar::Null)
{
    if (first == QChar::Null) {
        first = source[start];
        start += direction;
    }

    Q_ASSERT(isOpenBracket(first, direction));

    QStack<QChar> brackets;
    brackets.push(first);

    for (int i = start; i >= 0 && i < source.size(); i += direction) {
        if (isOpenBracket(source[i], direction))
            brackets.push(source[i]);
        else if (isCloseBracket(source[i], direction)) {
            if (brackets.size() > 0 && isClosing(brackets.top(), source[i], direction))
                brackets.pop();
            else // Brace sequence is incorrect
                return -1;
        }

        if (brackets.size() == 0)
            return i;
    }

    // Close brace not found
    return -1;
}

class MathWriter
{
public:
    static QString pass(const QString &source);
    static QVector<QPair<QString, QString>> snippetList();
    static QString applyParameters(QString source);
    static QString applyMatrices(QString source);
    static QString applyFractions(QString source);
    static QString applySnippets(QString source, const QVector<QPair<QString, QString>> &dict);
    static QString apply(QString source, const QVector<QPair<QString, QString>> &dict);
};

#endif // MATHWRITER_H
