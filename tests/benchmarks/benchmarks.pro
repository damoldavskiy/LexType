QT += testlib widgets

CONFIG += testcase

SOURCES += \
    ..\..\src\editor\stringgapbuffer.cpp \
    ..\..\src\editor\linetracker.cpp \
    ..\..\src\editor\line.cpp \
    ..\..\src\editor\cachedfont.cpp \
    ..\..\src\editor\markupmodel.cpp \
    ..\..\src\editor\text.cpp \
    ..\..\src\editor\memorydata.cpp \
    main.cpp

HEADERS += \
    ..\..\src\editor\gapbuffer.h \
    ..\..\src\editor\stringgapbuffer.h \
    ..\..\src\editor\linetracker.h \
    ..\..\src\editor\word.h \
    ..\..\src\editor\line.h \
    ..\..\src\editor\cachedfont.h \
    ..\..\src\editor\markupmodel.h \
    ..\..\src\editor\text.h \
    ..\..\src\editor\memorydata.h \
    tst_text.h
