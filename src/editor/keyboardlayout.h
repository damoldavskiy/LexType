#ifndef KEYBOARDLAYOUT_H
#define KEYBOARDLAYOUT_H

#include <QString>

/**
 * @brief Used for switching user's keyboard layout
 */
class KeyboardLayout
{
public:
    static QChar pass(QChar symbol);

private:
    static QString _latin;
    static QString _cyril;
};

#endif // KEYBOARDLAYOUT_H
