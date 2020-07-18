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

namespace Viewer {

class ScrollArea : public QWidget
{
public:
    ScrollArea(QWidget *parent = 0);
    ~ScrollArea();

    void zoomIn(QPointF center = { -1, -1 });
    void zoomOut(QPointF center = { -1, -1 });
    void zoom(QPointF center, qreal factor);

    void load(Poppler::Document *document);

protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);

    QSize pageSize(int number) const;
    void clear();
    void updateShifts();
    void recolor(QImage *image) const;
    QVector<SplitArea> splitImage(int page, QSize psize, int x, int y, int w, int h);

private:
    Poppler::Document *_document = nullptr;
    QVector<Poppler::Page*> _pages;

    qreal _xshift = 0;
    qreal _yshift = 0;

    qreal _dpi;
    qreal _zoomScale;

    qreal _scale; // Resolution
    qreal _pageShift;

    // Caching
    int _areaWidth = 512;
    int _areaHeight = 512;

    QQueue<SplitArea> _cache;
    int _maxCacheSize = 12;
};

}

#endif // SCROLLAREA_H
