#include "interval.h"

Interval::Interval(IntervalType type, bool math, bool display)
{
    setType(type);
    setMath(math);
    setDisplay(display);
}

IntervalType Interval::type() const
{
    return static_cast<IntervalType>(_data >> 2);
}

bool Interval::isMath() const
{
    return _data & 1;
}

bool Interval::isDisplay() const
{
    return _data & 2;
}

void Interval::setType(IntervalType type)
{
    _data &= 1 + 2;
    _data |= type << 2;
}

void Interval::setMath(bool math)
{
    _data &= 2 + (4 + 8 + 16);
    _data |= math;
}

void Interval::setDisplay(bool display)
{
    _data &= 1 + (4 + 8 + 16);
    _data |= display << 1;
}

bool operator ==(const Interval &lhs, const Interval &rhs)
{
    return lhs._data == rhs._data;
}

bool operator !=(const Interval &lhs, const Interval &rhs)
{
    return lhs._data != rhs._data;
}
