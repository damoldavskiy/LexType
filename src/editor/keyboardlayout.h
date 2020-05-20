#ifndef KEYBOARDLAYOUT_H
#define KEYBOARDLAYOUT_H

#include <QString>

/**
 * @brief Used for switching user's keyboard layout
 */
class KeyboardLayout
{
public:
    /**
     * @brief Inverses keyboard layout for the symbol
     * @param Initial symbol
     * @return Same symbol with reversed layout
     */
    static QChar pass(QChar symbol);

private:
    static QString _latin;
    static QString _cyril;
};

#endif // KEYBOARDLAYOUT_H
