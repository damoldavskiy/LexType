#ifndef FIGURETEXT_H
#define FIGURETEXT_H

#include "figure.h"

/**
 * @brief Text figure
 */
class FigureText : public Figure
{
public:
    bool release();
    void setEnd(QPointF end, bool modifier = false);
    void paint(QPainter *painter) const;
    QString latex() const;
    Figure* copy() const;
    void clear();

    /**
     * @brief Returns text of figure
     * @return Text
     */
    QString text() const;

    /**
     * @brief Sets text of figure
     * @param Text
     */
    void setText(const QString &value);

private:
    QString _text;
};

#endif // FIGURETEXT_H
