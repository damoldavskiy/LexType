#ifndef WORD_H
#define WORD_H

#include <QString>

struct Word
{
    int start = 0;
    qreal width = 0;
    QChar leading = '\0';
    QString text;
};

#endif // WORD_H
