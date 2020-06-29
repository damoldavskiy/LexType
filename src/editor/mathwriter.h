#ifndef MATHWRITER_H
#define MATHWRITER_H

#include <QString>

class MathWriter
{
public:
    static QString pass(const QString &source);
    static QString applyParameters(QString source);
    static QString applyMatrices(QString source);
    static QString applyFractions(QString source);
    static QString apply(const QString &source);
};

#endif // MATHWRITER_H
