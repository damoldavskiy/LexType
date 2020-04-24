#include <QtTest>

#include "../../src/editor/text.h"

class tst_Text : public QObject
{
    Q_OBJECT

private slots:
    inline void type();
    inline void typeStart();
    inline void line();
    inline void lineStart();
    inline void multiline();
    inline void removeLine();
    inline void removeAll();
    inline void editStart();
};

void tst_Text::type()
{
    QFont font;
    Text text(font);

    QBENCHMARK {
        text.insert(text.size(), QString('a'));
    }
}

void tst_Text::typeStart()
{
    QFont font;
    Text text(font);

    QBENCHMARK {
        text.insert(0, QString('a'));
    }
}

void tst_Text::line()
{
    QFont font;
    Text text(font);

    QBENCHMARK {
        text.insert(text.size(), "Simple text with length of 30!");
    }
}

void tst_Text::lineStart()
{
    QFont font;
    Text text(font);

    QBENCHMARK {
        text.insert(0, "Simple text with length of 30!");
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

void tst_Text::editStart()
{
    QFont font;
    Text text(font);

    for (int i = 0; i < 10000; ++i)
        text.insert(text.size(), "w");
    text.remove(0, 1);

    // TODO Theoretically, GapBuffer should work greatly faster
    // than QString, but it's really not. Something slows it down
    QBENCHMARK_ONCE {
        for (int i = 0; i < 1000; ++i)
            text.remove(0, 1);
    }
}
