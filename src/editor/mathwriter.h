#ifndef MATHWRITER_H
#define MATHWRITER_H

#include <QString>

/**
 * @brief LexType preprocessor (advanced math mode, fractions, etc.)
 */
class MathWriter
{
public:
    static QString pass(const QString &source);
    static QString applyFractions(QString source);
    static QString applyParameters(QString source);
    static QString apply(QString source);
};

#endif // MATHWRITER_H
