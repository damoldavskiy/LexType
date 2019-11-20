#include <QtTest>

#include "../../src/editor.h"

class tst_Editor : public QObject
{
    Q_OBJECT

private slots:
    inline void basic();
};

void tst_Editor::basic()
{
    Editor editor;

    QString text = "Hello, world! This is me";
    QTest::keyClicks(&editor, text);
    QCOMPARE(editor.text(), text);
}
