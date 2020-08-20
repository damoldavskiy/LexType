#ifndef LINETRACKER_H
#define LINETRACKER_H

#include <QVector>
#include <QString>
#include <QPair>

#include "line.h"

class LineTracker
{
public:
    LineTracker();

    void insert(int pos, const QString &text);
    void remove(int pos, int count);

    int find(int pos) const;

    QString mid(int pos, int count = -1) const;

    void setFont(const CachedFont *font);

    const Line &operator [](int line) const;
    int size() const;

private:
    void updateStarts(int line);

    QVector<Line> _lines;
    const CachedFont *_font;
};

#endif // LINETRACKER_H
