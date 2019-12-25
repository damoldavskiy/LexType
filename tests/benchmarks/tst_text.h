#include <QtTest>

#include "../../src/editor/text.h"

class tst_Text : public QObject
{
    Q_OBJECT

private slots:
    inline void line();
    inline void multiline();
    inline void removeLine();
    inline void removeAll();
};

void tst_Text::line()
{
    QFont font;
    Text text(font);

    QBENCHMARK {
        text.insert(text.size(), "Simple text with length of 30!");
    }
}

void tst_Text::multiline()
{
    QFont font;
    Text text(font);

    QBENCHMARK {
        text.insert(text.size(), "Simple line with length of 30\n");
    }
}

void tst_Text::removeLine()
{
    QFont font;
    Text text(font);

    for (int i = 0; i < 10000; ++i)
        text.insert(text.size(), "Simple line with length of 30\n");

    QBENCHMARK_ONCE {
        for (int i = 0; i < 10000; ++i)
            text.remove(0, 30);
    }
}

void tst_Text::removeAll()
{
    QFont font;
    Text text(font);

    for (int i = 0; i < 10000; ++i)
        text.insert(text.size(), "Simple line with length of 30\n");

    QBENCHMARK_ONCE {
        text.remove(0, 30 * 10000);
    }
}
