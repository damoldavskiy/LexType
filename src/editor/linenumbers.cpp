#include "linenumbers.h"

#include "math.h"
#include "styler.h"

LineNumbers::LineNumbers(QWidget *parent)
    : QWidget(parent)
{ }

void LineNumbers::clear()
{
    _values.clear();
    _current = -1;
    _maxWidth = 0;
}

void LineNumbers::setMax(int value)
{
    _maxWidth = fontMetrics().width(QString::number(value));
    setMaximumWidth(_leftShift + _maxWidth + _rightShift);
}

void LineNumbers::setCurrent(int value)
{
    _current = value;
}

void LineNumbers::add(qreal shift, int value)
{
    _values.append({ shift, value });
}

void LineNumbers::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.fillRect(0, 0, size().width(), size().height(), Styler::numbersBack());
    painter.setPen(Styler::numbersFore());

    for (const auto& pair : _values) {
        QString text = QString::number(pair.second);
        int textWidth = fontMetrics().width(text);

        if (pair.second == _current)
            painter.setPen(Styler::numberCurrent());

        painter.drawText(_leftShift + _maxWidth - textWidth, pair.first, text);

        if (pair.second == _current)
            painter.setPen(Styler::numbersFore());
    }
}
