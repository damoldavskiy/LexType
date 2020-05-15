#ifndef PAINTER_H
#define PAINTER_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QStack>

#include "figure.h"
#include "figurepath.h"

class Painter : public QWidget
{
    Q_OBJECT

public:
    Painter(QWidget *parent = 0);
    ~Painter();
    QString latex() const;
    Figure* figure() const;
    void setFigure(Figure* value);

signals:
    void released();

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
    void resizeEvent(QResizeEvent *event);

private:
    QPoint _mouse;

    QStack<Figure*> _figures;
    QStack<Figure*> _afterFigures;
    Figure* _current = new FigurePath;
};

#endif // PAINTER_H
