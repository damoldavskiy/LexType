#ifndef SCROLLAREA_H
#define SCROLLAREA_H

#include <QWidget>
#include <QPainter>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QQueue>
#include <poppler-qt5.h>

#include "splitarea.h"

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
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);

    void clear();
    void updateShifts();
    void recolor(QImage *image);
    QVector<SplitArea> splitImage(int page, QSize psize, int x, int y, int w, int h);

private:
    Poppler::Document *_document = nullptr;
    QVector<Poppler::Page*> _pages;

    int _xshift = 0;
    int _yshift = 0;

    int _dpi = 72;

    qreal _scale = 2;
    int _pageShift = 20;

    int _areaWidth = 512;
    int _areaHeight = 512;

    QQueue<SplitArea> _cache;
    int _maxCacheSize = 12;
};

#endif // SCROLLAREA_H
