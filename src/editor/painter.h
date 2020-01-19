#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QStack>

#include "figure.h"

class Painter : public QWidget
{
    Q_OBJECT

public:
    Painter(QWidget *parent = 0);
    ~Painter();
    QString latex() const;
    void setType(Figure::Type type);

public slots:
    void undo();
    void redo();

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPoint _mouse;
    bool _edit = false;

    QStack<Figure*> _figures;
    QStack<Figure*> _afterFigures;
    Figure::Type _type = Figure::Path;

    QColor _background = { 50, 50, 50 };
    QColor _foreground = { 240, 240, 240 };
};

#endif // PAINTER_H
