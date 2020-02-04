#ifndef FIGURETEXT_H
#define FIGURETEXT_H

#include "figure.h"

class FigureText : public Figure
{
public:
    void setEnd(QPointF end, bool modifier = false);
    void paint(QPainter *painter) const;
    QString latex() const;
    Figure* copy() const;
    void clear();

    QString text() const;
    void setText(const QString &value);

private:
    QString _text;
};

#endif // FIGURETEXT_H
