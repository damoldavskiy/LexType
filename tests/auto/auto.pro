QT += testlib widgets

CONFIG += testcase

SOURCES += \
    ..\..\src\editor\linenumbers.cpp \
    ..\..\src\editor\linetracker.cpp \
    ..\..\src\editor\markupmodel.cpp \
    ..\..\src\editor\text.cpp \
    ..\..\src\editor\memorydata.cpp \
    ..\..\src\editor\keyboardlayout.cpp \
    ..\..\src\editor\editor.cpp \
    ..\..\src\editor\styler.cpp \
    ..\..\src\editor\mathwriter.cpp \
    main.cpp

HEADERS += \
    ..\..\src\editor\linenumbers.h \
    ..\..\src\editor\linetracker.h \
    ..\..\src\editor\markupmodel.h \
    ..\..\src\editor\text.h \
    ..\..\src\editor\memorydata.h \
    ..\..\src\editor\keyboardlayout.h \
    ..\..\src\editor\editor.h \
    ..\..\src\editor\styler.h \
    ..\..\src\editor\mathwriter.h \
    tst_editor.h \
    tst_text.h \
    tst_mathwriter.h
