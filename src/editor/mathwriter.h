#ifndef MATHWRITER_H
#define MATHWRITER_H

#include <QString>

class MathWriter
{
public:
    static QString pass(const QString &source);
    static QString applyFractions(QString source);
    static QString apply(QString source);
};

#endif // MATHWRITER_H
