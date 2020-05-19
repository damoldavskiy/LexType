#ifndef LINETRACKER_H
#define LINETRACKER_H

#include <QVector>
#include <QString>
#include <QPair>

#include "range.h"

/**
 * @brief Structure for tracking lines in text
 */
class LineTracker
{
public:
    explicit LineTracker(int n);
    QPair<int, int> insert(int pos, const QString &text);
    void insertText(int line, int count);
    void insertLine(int line, int shift);
    QPair<int, int> remove(int pos, int count);
    int find(int pos) const;
    const Range &operator [](int line) const;
    int size() const;

private:
    QVector<Range> _lines;
};

#endif // LINETRACKER_H
