#ifndef ACTION_H
#define ACTION_H

#include "QString"

/**
 * @brief Used to save information about user changes in editor
 */
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
