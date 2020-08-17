#ifndef WORD_H
#define WORD_H

#include <QChar>

struct Word
{
    int start = 0;
    int size = 0;
    QChar leading = '\0';
};

#endif // WORD_H
