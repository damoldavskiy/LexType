QT += widgets

INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -L/usr/lib -lpoppler-qt5

SOURCES += \
    ../editor/styler.cpp \
    ../editor/pathwatcher.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    ../editor/styler.h \
    ../editor/pathwatcher.h \
    mainwindow.h \

