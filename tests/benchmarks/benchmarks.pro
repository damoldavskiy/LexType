QT += testlib widgets

CONFIG += testcase

SOURCES += \
    ..\..\src\editor\gapbuffer.cpp \
    ..\..\src\editor\linetracker.cpp \
    ..\..\src\editor\word.cpp \
    ..\..\src\editor\line.cpp \
    ..\..\src\editor\markupmodel.cpp \
    ..\..\src\editor\text.cpp \
    ..\..\src\editor\memorydata.cpp \
    main.cpp

HEADERS += \
    ..\..\src\editor\gapbuffer.h \
    ..\..\src\editor\linetracker.h \
    ..\..\src\editor\word.h \
    ..\..\src\editor\line.h \
    ..\..\src\editor\markupmodel.h \
    ..\..\src\editor\text.h \
    ..\..\src\editor\memorydata.h \
    tst_text.h
