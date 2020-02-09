QT += testlib widgets

CONFIG += testcase

SOURCES += \
    ..\..\src\editor\linetracker.cpp \
    ..\..\src\editor\markupmodel.cpp \
    ..\..\src\editor\text.cpp \
    ..\..\src\editor\memorydata.cpp \
    main.cpp

HEADERS += \
    ..\..\src\editor\linetracker.h \
    ..\..\src\editor\markupmodel.h \
    ..\..\src\editor\text.h \
    ..\..\src\editor\memorydata.h \
    tst_text.h
