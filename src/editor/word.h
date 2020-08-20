#ifndef WORD_H
#define WORD_H

#include "stringgapbuffer.h"

struct Word
{
    int start = 0;
    qreal width = 0;
    QChar leading = '\0';
    StringGapBuffer text;
};

#endif // WORD_H
