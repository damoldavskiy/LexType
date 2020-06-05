#ifndef ACTION_H
#define ACTION_H

#include "QString"

struct Action
{
    enum Type
    {
        Insert,
        Remove
    };

    int index;
    QString text;
    Type type;
};

#endif // ACTION_H
