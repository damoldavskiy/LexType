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
    /**
     * @brief Constructs line tracker
     * @param Initial number of lines
     */
    explicit LineTracker(int n);

    /**
     * @brief Updates data by inserting text in the position
     * @param Position of insertion
     * @param Text
     * @return First line and overall count of lines modified by insertion
     */
    QPair<int, int> insert(int pos, const QString &text);

    /**
     * @brief Inserts pecified count of symbols into line
     * @param Line to modificate
     * @param Count of symbols
     */
    void insertText(int line, int count);

    /**
     * @brief Inserts breaking line symbol
     * @param Line to break
     * @param Shift from start of line
     */
    void insertLine(int line, int shift);

    /**
     * @brief Updates data by removing text from the position
     * @param Position of deletion
     * @param Count of deleted symbols
     * @return First line and overall count of lines modified by insertion
     */
    QPair<int, int> remove(int pos, int count);

    /**
     * @brief Searches the line by symbol position
     * @param Symbol position
     * @return Line number
     */
    int find(int pos) const;

    /**
     * @brief Searches the first symbol position of line
     * @param Line to search
     * @return First symbol position and count of symbols at line
     */
    const Range &operator [](int line) const;

    /**
     * @brief Returns count of lines
     * @return Count
     */
    int size() const;

private:
    QVector<Range> _lines;
};

#endif // LINETRACKER_H
