QT += widgets

unix {
    INCLUDEPATH += /usr/include/poppler/qt5
    LIBS += -L/usr/lib -lpoppler-qt5
}

win32 {
    INCLUDEPATH += D:/lib/poppler/qt5
    LIBS += -LD:/lib/poppler/ -lpoppler-qt5
}

SOURCES += \
    ../editor/styler.cpp \
    ../editor/pathwatcher.cpp \
    main.cpp \
    mainwindow.cpp \
    scrollarea.cpp

HEADERS += \
    ../editor/styler.h \
    ../editor/pathwatcher.h \
    mainwindow.h \
    scrollarea.h

