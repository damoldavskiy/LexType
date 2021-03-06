#include "cachedfont.h"

#include "math.h"

QVector<QStaticText> cacheText(int start, int end, const QFont &font, const QFontMetricsF &metrics)
{
    QVector<QStaticText> values(end - start + 1);
    for (int i = 0; i < end - start; ++i) {
        values[i] = QStaticText(static_cast<QChar>(start + i));
        values[i].setTextWidth(metrics.width(static_cast<QChar>(start + i)));
        values[i].prepare(QTransform(), font);
    }
    return values;
}

CachedFont::CachedFont(const QFont &font, int tabCount)
    : _font(font), _metrics(font), _tabWidth(width('x') * tabCount), _height(_metrics.height()), _ascent(_metrics.ascent())
{
    _cachedText.add(0x0000, cacheText(0x0000, 0x00FF, _font, _metrics)); // ASCII
    _cachedText.add(0x0400, cacheText(0x0400, 0x04FF, _font, _metrics)); // Cyrillic
}

QStaticText CachedFont::get(QChar symbol) const
{
    int code = symbol.unicode();

    int index = _cachedText.find(code);
    if (index == -1) {
        QStaticText text(symbol);
        text.setTextWidth(_metrics.width(symbol));
        return text;
    }

    return _cachedText.get(index, code);
}

qreal CachedFont::height() const
{
    return _height;
}

qreal CachedFont::ascent() const
{
    return _ascent;
}

qreal CachedFont::width(QChar symbol) const
{
    return Math::max(0.0, get(symbol).textWidth());
}

qreal CachedFont::tabWidth() const
{
    return _tabWidth;
}
