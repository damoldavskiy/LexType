#ifndef CACHEDFONT_H
#define CACHEDFONT_H

#include <QFont>
#include <QFontMetrics>
#include <QStaticText>

#include "multirange.h"

class CachedFont
{
public:
    CachedFont(const QFont &font, int tabCount);
    CachedFont(const CachedFont&) = delete;

    QStaticText get(QChar symbol) const;

    qreal height() const;
    qreal ascent() const;
    qreal width(QChar symbol) const;
    qreal tabWidth() const;

private:
    QFont _font;
    QFontMetricsF _metrics;
    MultiRange<QStaticText> _cachedText;
    qreal _tabWidth;
};

#endif // CACHEDFONT_H
