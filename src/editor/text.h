#ifndef TEXT_H
#define TEXT_H

#include <QString>
#include <QVector>
#include <QFontMetrics>
#include <QStaticText>
#include <QSet>
#include <QMap>

#include "linetracker.h"
#include "maxvector.h"
#include "multirange.h"
#include "memorydata.h"
#include "markupmodel.h"

class Text
{
public:
    Text(const QFont &font, int tabCount);

    void setFont(const QFont &font, int tabCount);

    void insert(int pos, const QString &text);
    void insertLinesAdjust(int line, int added);

    void remove(int pos, int count);
    void removeLinesAdjust(int line, int removed);

    void setWindowWidth(qreal width);
    void updateLineBreaks(int line, int count);

    int find(int pos, const QString &substring, bool matchCase) const;

    int undo();
    int redo();
    bool canUndo() const;
    bool canRedo() const;

    QString mid(int pos, int count = -1) const;
    QChar operator [](int pos) const;

    int findLine(int pos) const;
    int lineStart(int line) const;
    int lineSize(int line) const;
    qreal lineHeight(int line) const;
    qreal linesHeightBefore(int line) const;
    qreal lineWidth(int line) const;

    QPair<int, int> findLineWord(int pos) const;

    bool isBreak(int pos) const;
    int breaksInline(int pos) const;
    int visualLinesCount(int line) const;

    int size() const;
    int lineCount() const;

    qreal width() const;
    int visualLinesCount() const;
    qreal fontHeight() const;
    qreal fontAscent() const;
    qreal advanceWidth(qreal left, int pos) const;
    qreal advanceLeading(qreal left, const Word &word) const;

    QString text() const;
    QStaticText text(int pos) const;
    void updateMarkup(const Action &action, bool reverseType = false);
    Interval markup(int pos) const;

    int findPos(qreal x, qreal y, bool exact = false) const;
    QPointF findShift(int pos) const;

private:
    CachedFont _font;
    MemoryData _tracker;
    MarkupModel _markup;
    MaxVector<qreal> _widths;

    QMap<int, QSet<int>> _lineBreaks;
    qreal _windowWidth = qInf();

    mutable int _lastLine = -1;
    mutable int _lastWord = -1;
};

#endif // TEXT_H
