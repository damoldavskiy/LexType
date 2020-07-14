#include "scrollarea.h"

#include <QtConcurrent/QtConcurrent>

#include "../editor/math.h"
#include "../editor/styler.h"

namespace Viewer {

ScrollArea::ScrollArea(QWidget *parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
}

ScrollArea::~ScrollArea()
{
    clear();
}

void ScrollArea::zoomIn(QPointF center)
{
    zoom(center, _zoomScale);
}

void ScrollArea::zoomOut(QPointF center)
{
    zoom(center, 1 / _zoomScale);
}

void ScrollArea::zoom(QPointF center, qreal factor)
{
    if (center == QPointF { -1, -1 }) {
        center.setX(width() / 2);
        center.setY(height() / 2);
    }

    int newdpi = _dpi * factor;
    if (newdpi < _minDpi || newdpi > _maxDpi)
        return;

    _dpi = newdpi;
    _xshift = _xshift * factor + center.x() * factor - center.x();
    _yshift = _yshift * factor + center.y() * factor - center.y();

    _cache.clear();
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
        QSize size = pageSize(i);

        if (top - _yshift > height())
            break;

        if (top - _yshift + size.height() > 0) {
            int x = -_xshift;
            int y = top - _yshift;

            int rx = qMax(0, -x);
            int ry = qMax(0, -y);

            int rw = qMax(0, qMin(size.width(), width() - x) - rx);
            int rh = qMax(0, qMin(size.height(), height() - y) - ry);

            for (const SplitArea& area : splitImage(i, size, rx, ry, rw, rh))
                painter.drawImage(x + area.dx, y + area.dy, area.image);
        }

        top += size.height() + _pageShift;
    }
}

void ScrollArea::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0)
            zoomIn(event->posF());
        else
            zoomOut(event->posF());
        return;
    }

    _xshift -= event->angleDelta().x();
    _yshift -= event->angleDelta().y();

    updateShifts();
    update();
}

void ScrollArea::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Down:
        _yshift += 40;
        break;
    case Qt::Key_Up:
        _yshift -= 40;
        break;
    case Qt::Key_Left:
        if (_pages.size() > 0) {
            QSize size = pageSize(0);
            int n = (_yshift - 1) / (size.height() + _pageShift);
            _yshift = n * size.height() + n * _pageShift;
        }
        break;
    case Qt::Key_Right:
        if (_pages.size() > 0) {
            QSize size = pageSize(0);
            int n = (_yshift / (size.height() + _pageShift)) + 1;
            if (n == _pages.size())
                return;
            _yshift = n * size.height() + n * _pageShift;
        }
        break;
    case Qt::Key_Home:
        _yshift = 0;
        break;
    case Qt::Key_End:
        _yshift = INT_MAX;
    }

    updateShifts();
    update();
}

void ScrollArea::resizeEvent(QResizeEvent *)
{
    updateShifts();
}

QSize ScrollArea::pageSize(int number)
{
    return _pages[number]->pageSize() * _dpi / 72;
}

void ScrollArea::clear()
{
    for (Poppler::Page *page : _pages)
        delete page;
    _pages.clear();

    if (_document != nullptr)
        delete _document;
    _document = nullptr;

    _cache.clear();
}

void ScrollArea::updateShifts()
{
    if (_pages.empty()) {
        _xshift = 0;
        _yshift = 0;
    } else {
        QSize size = pageSize(0);
        int w = size.width();
        int n = _document->numPages();
        int h = n * size.height() + (n - 1) * _pageShift;

        if (w < width())
            _xshift = (w - width()) / 2;
        else
            Math::limit(_xshift, 0, w - width());

        if (h < height())
            _yshift = 0;
        else
            Math::limit(_yshift, 0, h - height());
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

    int threadsCount = QThreadPool::globalInstance()->maxThreadCount();
    QVector<QFuture<void>> futures;

    for (int i = 0; i < threadsCount; ++i) {
        int from = size / threadsCount * i;
        int to = from + size / threadsCount;
        futures.append(QtConcurrent::run([=] () {
            for (int j = from; j < to; ++j) {
                uchar cur = begin[j];
                begin[j] = backArray[j % 4] * cur / 255 + foreArray[j % 4] * (255 - cur) / 255;
            }
        }));
    }

    for (QFuture<void> &future : futures)
        future.waitForFinished();
}

QVector<SplitArea> ScrollArea::splitImage(int page, QSize psize, int x, int y, int w, int h)
{
    int xs = x / _areaWidth;
    int xe = (x + w) / _areaWidth;
    int ys = y / _areaHeight;
    int ye = (y + h) / _areaHeight;

    QVector<SplitArea> result;

    QVector<QPair<int, int>> params;
    for (int xi = xs; xi <= xe; ++xi)
        for (int yi = ys; yi <= ye; ++yi) {
            QQueue<SplitArea>::const_iterator it;
            for (it = _cache.cbegin(); it != _cache.cend(); ++it)
                if (it->page == page && it->dx / _areaWidth == xi && it->dy / _areaHeight == yi)
                    break;

            if (it != _cache.cend())
                result.append(*it);
            else
                params.append({ xi, yi });
        }

    QVector<SplitArea> areas = QtConcurrent::blockingMapped(params, std::function<SplitArea(QPair<int, int>)>([=] (QPair<int, int> pair) -> SplitArea {
        int xi = pair.first;
        int yi = pair.second;

        int dx = xi * _areaWidth;
        int dy = yi * _areaHeight;

        QImage image = _pages[page]->renderToImage(_dpi * _scale, _dpi * _scale, dx * _scale, dy * _scale, qMin(_areaWidth, psize.width() - dx) * _scale, qMin(_areaHeight, psize.height() - dy) * _scale);
        if (_scale != 1)
            image = image.scaled(image.width() / _scale, image.height() / _scale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        recolor(&image);

        return { std::move(image), dx, dy, page };
    }));

    for (const SplitArea& area : areas) {
        result.append(area);

        _cache.append(area);
        if (_cache.size() > _maxCacheSize)
            _cache.removeFirst();
    }

    return result;
}

}
