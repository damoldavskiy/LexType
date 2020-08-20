QT += testlib widgets

CONFIG += testcase

SOURCES += \
    ..\..\src\editor\gapbuffer.cpp \
    ..\..\src\editor\linenumbers.cpp \
    ..\..\src\editor\linetracker.cpp \
    ..\..\src\editor\line.cpp \
    ..\..\src\editor\cachedfont.cpp \
    ..\..\src\editor\markupmodel.cpp \
    ..\..\src\editor\text.cpp \
    ..\..\src\editor\memorydata.cpp \
    ..\..\src\editor\keyboardlayout.cpp \
    ..\..\src\editor\editor.cpp \
    ..\..\src\editor\styler.cpp \
    ..\..\src\editor\snippet.cpp \
    ..\..\src\editor\snippetmanager.cpp \
    ..\..\src\editor\mathwriter.cpp \
    main.cpp

HEADERS += \
    ..\..\src\editor\gapbuffer.h \
    ..\..\src\editor\linenumbers.h \
    ..\..\src\editor\linetracker.h \
    ..\..\src\editor\word.h \
    ..\..\src\editor\line.h \
    ..\..\src\editor\cachedfont.h \
    ..\..\src\editor\markupmodel.h \
    ..\..\src\editor\text.h \
    ..\..\src\editor\memorydata.h \
    ..\..\src\editor\keyboardlayout.h \
    ..\..\src\editor\editor.h \
    ..\..\src\editor\styler.h \
    ..\..\src\editor\snippet.h \
    ..\..\src\editor\snippetmanager.h \
    ..\..\src\editor\mathwriter.h \
    tst_editor.h \
    tst_text.h \
    tst_mathwriter.h
