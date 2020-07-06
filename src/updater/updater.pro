QT += network

CONFIG += console

DEFINES += QT_DEPRECATED_WARNINGS

unix {
    INCLUDEPATH += /usr/include/quazip5
    LIBS += -L/usr/lib -lquazip5
}

win32 {
    INCLUDEPATH += D:/lib/quazip/quazip
    LIBS += -LD:/lib/quazip -lquazip
}

SOURCES += \
    main.cpp \
    program.cpp

HEADERS += \
    program.h
