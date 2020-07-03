#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <poppler-qt5.h>

class ScrollArea : public QWidget
{
public:
    ScrollArea(QWidget *parent = 0);
    ~ScrollArea();

    int dpi();
    void setDpi(int value);

    void load(Poppler::Document *document);

protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);

    void clear();
    void updateShifts();
    void recolor(QImage *image);

private:
    Poppler::Document *_document = nullptr;
    QVector<Poppler::Page*> _pages;

    int _xshift = 0;
    int _yshift = 0;

    int _dpi = 72;

    qreal _scale = 2;
    int _pageShift = 20;
};

#endif // SCROLLAREA_H
