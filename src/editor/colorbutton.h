#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

class ColorButton : public QPushButton
{
    Q_OBJECT

public:
    ColorButton(QWidget *parent = nullptr);

    QColor color();
    void setColor(QColor color);

signals:
    void selected(QColor color);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QColor _color;
};

#endif // COLORBUTTON_H
