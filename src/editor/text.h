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

/**
 * @brief Includes all algorithms for editor implementation except GUI part
 */
class Text
{
public:
    /**
     * @brief Constructs text
     * @param Fond of text srored
     */
    Text(const QFont &font);

    /**
     * @brief Updates current font
     * @param Font
     */
    void setFont(const QFont &font);

    /**
     * @brief Cleares cache, then caches ASCII and cyrillic symbols
     */
    void cache();

    /**
     * @brief Insert text to position and applies markup
     * @param Position of insertion
     * @param Text to insert
     */
    void insert(int pos, const QString &text);

    /**
     * @brief Updates lines after insertion
     * @param Position
     * @param Text to insert
     */
    void insertLinesAdjust(int pos, const QString &text);

    /**
     * @brief Removes text from position and applies markup
     * @param Position of deletion
     * @param Count of symbols to delete
     */
    void remove(int pos, int count);

    /**
     * @brief Updates lines after deletion
     * @param Position
     * @param Count of symbols
     */
    void removeLinesAdjust(int pos, int count);

    /**
     * @brief Searches substring in the text
     * @param Initial position
     * @param Substring to find
     * @param Match case
     * @return Index of substring, -1 if not found
     */
    int find(int pos, const QString &substring, bool matchCase) const;

    /**
     * @brief Undo last action
     * @return New caret position
     */
    int undo();

    /**
     * @brief Redo last action
     * @return New caret position
     */
    int redo();

    /**
     * @brief Checks whether it is posiible to undo or not
     * @return True if possible, false otherwise
     */
    bool canUndo() const;

    /**
     * @brief Checks whether it is possible to redo or not
     * @return True if possible, false otherwise
     */
    bool canRedo() const;

    /**
     * @brief Returns substring of text
     * @param Start position
     * @param Count of symbols
     * @return String
     */
    QString mid(int pos, int count) const;

    /**
     * @brief Returns symbol at position
     * @param Position
     * @return Symbol
     */
    QChar operator [](int pos) const;

    /**
     * @brief Searches line number by position
     * @param Position
     * @return Line
     */
    int findLine(int pos) const;

    /**
     * @brief Searches line start position
     * @param Line
     * @return First line symbol position
     */
    int lineStart(int line) const;

    /**
     * @brief Returns size of specified line
     * @param Line
     * @return Size
     */
    int lineSize(int line) const;

    /**
     * @brief Returns length of text stored
     * @return Length
     */
    int size() const;

    /**
     * @brief Returns count of lines in text
     * @return Count of lines
     */
    int lineCount() const;

    /**
     * @brief Calculates overall width of text with current font
     * @return Width (in pixels)
     */
    qreal width() const;

    /**
     * @brief Returns current font height
     * @return Height (in pixels)
     */
    qreal fontHeight() const;

    /**
     * @brief Returns current font ascent
     * @return Ascent (in pixels)
     */
    qreal fontAscent() const;

    /**
     * @brief Calculates shift from first symbol to specified
     * @param Shift from first symbol to last symbol before specified
     * @param Position of specified symbol
     * @return Shift (in pixels)
     */
    qreal advanceWidth(qreal left, int pos) const;

    /**
     * @brief Calculates width of line
     * @param Line index
     * @return Line width (in pixels)
     */
    qreal lineWidth(int line) const;

    /**
     * @brief Returns stored text
     * @return Text
     */
    QString text() const;

    /**
     * @brief Returns static text object by symbol at position. Cache may be used
     * @param Symbol position
     * @return Static text
     */
    QStaticText text(int pos) const;

    /**
     * @brief Updates markup starting from position
     * @param Start position
     */
    void updateMarkup(int pos);

    /**
     * @brief Returns markup type at position
     * @param Position
     * @return Markup type
     */
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
