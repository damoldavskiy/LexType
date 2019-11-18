#include <QtTest>

#include "../src/editor.h"

class tst_Editor : public QObject
{
    Q_OBJECT

private slots:
    void basicTestCase()
    {
        Editor editor;

        QString text = "Hello, world! This is me";
        QTest::keyClicks(&editor, text);
        QCOMPARE(editor.text(), text);
    }
};
