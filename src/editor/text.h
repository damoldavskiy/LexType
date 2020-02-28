#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include <QVector>
#include <QFontMetrics>
#include <QStaticText>

#include "linetracker.h"
#include "maxvector.h"
#include "multirange.h"
#include "memorydata.h"
#include "markupmodel.h"

class Text
{
public:
    Text(const QFont &font);

    void setFont(const QFont &font);
    void cache();

    void insert(int pos, const QString &text);
    void insertLinesAdjust(int pos, const QString &text);
    void remove(int pos, int count);
    void removeLinesAdjust(int pos, int count);
    int undo();
    int redo();
    bool canUndo() const;
    bool canRedo() const;
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
    QStaticText text(int pos) const;

    void updateMarkup(int pos);
    Interval markup(int pos) const;

private:
    MemoryData _data;
    LineTracker _tracker;
    MarkupModel _markup;
    MaxVector<qreal> _widths;
    QFontMetricsF _fm;
    qreal _tabWidth;
    MultiRange<QStaticText> _cachedText;
};

#endif // TEXT_H
