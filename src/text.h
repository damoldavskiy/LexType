#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include <QVector>
#include <QFontMetrics>

#include "linetracker.h"
#include "maxvector.h"

class Text
{
public:
    Text(const QFont &font);

    void insert(int pos, const QString &text);
    void remove(int pos, int count);
    QString mid(int pos, int count) const;

    QChar operator [](int pos) const;
    int findLine(int pos) const;
    int lineStart(int line) const;
    int lineSize(int line) const;

    int size() const;
    int lineCount() const;

    qreal width() const;

    qreal fontHeight() const;
    qreal fontAscent() const;

    qreal advanceWidth(qreal left, int pos) const;
    qreal lineWidth(int line) const;

    QString text() const;

private:
    QString _data;
    LineTracker _tracker;
    MaxVector<qreal> _widths;
    QFontMetricsF _fm;
    qreal _tabWidth;
    QVector<qreal> _cachedWidths;
};

#endif // TEXT_H
