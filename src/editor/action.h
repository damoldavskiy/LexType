#ifndef ACTION_H
#define ACTION_H

#include "QString"

struct Action
{
    enum Type
    {
        INSERT,
        REMOVE
    };

    int index;
    QString text;
    Type type;
};

#endif // ACTION_H
