#ifndef INTERVAL_H
#define INTERVAL_H

#include "intervaltype.h"

#include <QChar>

class Interval
{
public:
    explicit Interval(IntervalType type = IntervalType::Regular, bool math = false, bool display = false);

    IntervalType type() const;
    bool isMath() const;
    bool isDisplay() const;
    bool isEscape() const;

    void setType(IntervalType type);
    void setMath(bool math);
    void setDisplay(bool display);
    void setEscape(bool escape);

    Interval& update(QChar last, QChar cur);

    friend bool operator ==(const Interval &lhs, const Interval &rhs);
    friend bool operator !=(const Interval &lhs, const Interval &rhs);

private:
    unsigned char _data;
};

#endif // INTERVAL_H
