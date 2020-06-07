#include "colorbutton.h"

#include <QColorDialog>
#include <QPainter>

ColorButton::ColorButton(QWidget *parent)
    : QPushButton(parent)
{
    connect(this, &QPushButton::clicked, this, [this] () {
        QColor color = QColorDialog::getColor(_color);
        if (color.isValid()) {
            _color = color;
            selected(color);
        }
    });
}

QColor ColorButton::color()
{
    return _color;
}

void ColorButton::setColor(QColor color)
{
    _color = color;
    repaint();
}

void ColorButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);

    QPainter painter(this);
    int width = size().width();
    int height = size().height();

    painter.fillRect(2, 2, width - 4, height - 4, _color);
}
