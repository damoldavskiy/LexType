#include "scrollarea.h"

#include <QtMath>

#include "../editor/styler.h"

ScrollArea::ScrollArea(QWidget *parent)
    : QWidget(parent)
{ }

ScrollArea::~ScrollArea()
{
    clear();
}

int ScrollArea::dpi()
{
    return _dpi;
}

void ScrollArea::setDpi(int value)
{
    _dpi = value;
    updateShifts();
    update();
}

void ScrollArea::load(Poppler::Document *document)
{
    if (document == nullptr)
        return;

    clear();

    _document = document;
    for (int i = 0; i < _document->numPages(); ++i)
        _pages.append(_document->page(i));

    updateShifts();
    update();
}

void ScrollArea::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.fillRect(0, 0, width(), height(), Styler::get<QColor>("viewer-back"));

    if (_document == nullptr)
        return;

    int top = 0;
    for (int i = 0; i < _pages.size(); ++i) {
        QSize size = _pages[i]->pageSize() * _dpi / 72;

        if (top - _yshift > height())
            break;

        if (top - _yshift + size.height() > 0) {
            int x = (width() - size.width()) / 2 - _xshift;
            int y = top - _yshift;

            int rx = qMax(0, -x);
            int ry = qMax(0, -y);

            int rw = qMax(0, qMin(size.width(), width() - x) - rx);
            int rh = qMax(0, qMin(size.height(), height() - y) - ry);

            QImage image = _pages[i]->renderToImage(_dpi * _scale, _dpi * _scale, rx * _scale, ry * _scale, rw * _scale, rh * _scale);
            if (_scale != 1)
                image = image.scaled(image.width() / _scale, image.height() / _scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            recolor(&image);

            painter.drawImage(x + rx, y + ry, image);
        }

        top += size.height() + _pageShift;
    }
}

void ScrollArea::wheelEvent(QWheelEvent *event)
{
    _xshift -= event->angleDelta().x();
    _yshift -= event->angleDelta().y();

    updateShifts();
    update();
}

void ScrollArea::clear()
{
    for (Poppler::Page *page : _pages)
        delete page;
    _pages.clear();

    if (_document != nullptr)
        delete _document;
    _document = nullptr;
}

void ScrollArea::updateShifts()
{
    if (_pages.empty()) {
        _xshift = 0;
        _yshift = 0;
    } else {
        QSize size = _pages[0]->pageSize() * _dpi / 72;
        int w = size.width();
        int n = _document->numPages();
        int h = n * size.height() + (n - 1) * _pageShift;

        // TODO Use limit function
        if (w < width())
            _xshift = 0;
        else {
            if (_xshift > w / 2 - width() / 2)
                _xshift = w / 2 - width() / 2;
            if (_xshift < -(w / 2 - width() / 2))
                _xshift = -(w / 2 - width() / 2);
        }

        if (h < height())
            _yshift = 0;
        else {
            if (_yshift < 0)
                _yshift = 0;
            if (_yshift > h - height())
                _yshift = h - height();
        }
    }
}

void ScrollArea::recolor(QImage *image)
{
    QColor back = Styler::get<QColor>("viewer-paper");
    QColor fore = Styler::get<QColor>("viewer-text");
    uchar backArray[4] = { static_cast<uchar>(back.blue()), static_cast<uchar>(back.green()), static_cast<uchar>(back.red()), 0 };
    uchar foreArray[4] = { static_cast<uchar>(fore.blue()), static_cast<uchar>(fore.green()), static_cast<uchar>(fore.red()), 0 };

    int size = image->byteCount();
    uchar *begin = image->bits();

    // TODO Parallel
    for (int i = 0; i < size; ++i) {
        uchar cur = begin[i];
        begin[i] = backArray[i % 4] * cur / 255 + foreArray[i % 4] * (255 - cur) / 255;
    }
}
