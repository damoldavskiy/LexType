QT += testlib widgets

CONFIG += testcase

SOURCES += \
    ..\..\src\editor\linetracker.cpp \
    ..\..\src\editor\text.cpp \
    ..\..\src\editor\memorydata.cpp \
    ..\..\src\editor\editor.cpp \
    ..\..\src\editor\styler.cpp \
    main.cpp

HEADERS += \
    ..\..\src\editor\linetracker.h \
    ..\..\src\editor\text.h \
    ..\..\src\editor\editor.h \
    tst_editor.h \
    tst_text.h
