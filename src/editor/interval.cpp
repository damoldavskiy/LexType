#include "interval.h"

Interval::Interval(IntervalType type, bool math, bool display)
{
    setType(type);
    setMath(math);
    setDisplay(display);
}

IntervalType Interval::type() const
{
    return static_cast<IntervalType>(_data >> 3);
}

bool Interval::isMath() const
{
    return _data & 1;
}

bool Interval::isDisplay() const
{
    return _data & 2;
}

bool Interval::isEscape() const
{
    return _data & 4;
}

void Interval::setType(IntervalType type)
{
    _data &= 1 + 2 + 4;
    _data |= type << 3;
}

void Interval::setMath(bool math)
{
    _data &= 2 + 4 + (8 + 16 + 32);
    _data |= math;
}

void Interval::setDisplay(bool display)
{
    _data &= 1 + 4 + (8 + 16 + 32);
    _data |= display << 1;
}

void Interval::setEscape(bool escape)
{
    _data &= 1 + 2 + (8 + 16 + 32);
    _data |= escape << 2;
}

Interval& Interval::update(QChar last, QChar cur)
{
    IntervalType type = this->type();
    bool math = isMath();
    bool display = isDisplay();
    bool escape = isEscape();

    if (escape)
        escape = false;
    else if (last == '\\')
        escape = true;

    if ((cur == '%' && !escape) || (this->type() == IntervalType::Comment && cur != '\n')) {
        type = IntervalType::Comment;
        math = false;
        display = false;
    } else {
       if (cur == '`' && !escape) {
            if (math && last == '`' && !isEscape() && !display)
                display = true;
            else if (!math || (last == '`' && !isEscape()) || !display)
                math = !math;
            if (!math)
                display = false;
        }

        if ((cur == '`' && !escape) || math)
            type = IntervalType::Mathematics;
        else
            type = IntervalType::Regular;

        if (type != IntervalType::Mathematics) {
            if ((type == IntervalType::Regular && cur == '\\') || (this->type() == IntervalType::Command))
                type = IntervalType::Command;

            if (this->type() == IntervalType::Command) {
                if (cur.isLetter() && ((last == '\\' && !isEscape()) || last.isLetter()))
                    type = IntervalType::Command;
                else if (cur != '\\' && (!escape || (cur != '%' && cur != '`')))
                    type = IntervalType::Regular;
            }

            if (type == IntervalType::Regular && (cur == '{' || cur == '}' || cur == '$'))
                type = IntervalType::Special;
        }
    }

    setType(type);
    setMath(math);
    setDisplay(display);
    setEscape(escape);

    return *this;
}

bool operator ==(const Interval &lhs, const Interval &rhs)
{
    return lhs._data == rhs._data;
}

bool operator !=(const Interval &lhs, const Interval &rhs)
{
    return lhs._data != rhs._data;
}
