QT += testlib widgets

CONFIG += testcase

SOURCES += \
    ..\..\src\editor\linenumbers.cpp \
    ..\..\src\editor\linetracker.cpp \
    ..\..\src\editor\text.cpp \
    ..\..\src\editor\memorydata.cpp \
    ..\..\src\editor\editor.cpp \
    ..\..\src\editor\styler.cpp \
    main.cpp

HEADERS += \
    ..\..\src\editor\linenumbers.h \
    ..\..\src\editor\linetracker.h \
    ..\..\src\editor\text.h \
    ..\..\src\editor\memorydata.h \
    ..\..\src\editor\editor.h \
    ..\..\src\editor\styler.h \
    tst_editor.h \
    tst_text.h
