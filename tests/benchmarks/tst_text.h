#include <QtTest>

#include "../../src/text.h"

class tst_Text : public QObject
{
    Q_OBJECT

private slots:
    inline void oneline();
    inline void multiline();
    inline void removeline();
    inline void removeall();
};

void tst_Text::oneline()
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

void tst_Text::removeline()
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

void tst_Text::removeall()
{
    QFont font;
    Text text(font);

    for (int i = 0; i < 10000; ++i)
        text.insert(text.size(), "Simple line with length of 30\n");

    QBENCHMARK_ONCE {
        text.remove(0, 30 * 10000);
    }
}
